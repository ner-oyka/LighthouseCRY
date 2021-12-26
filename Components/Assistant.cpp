//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"
#include "Assistant.h"

#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/Utils/EnumFlags.h>
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>
#include <CrySchematyc/ResourceTypes.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CryCore/StaticInstanceList.h>

#include <CryMath/Random.h>

#include <CryDynamicResponseSystem/IDynamicResponseSystem.h>

#include "Framework/PlayerController.h"

#include <Animation/PoseModifier/IKTorsoAim.h>

static void RegisterAssistantComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CAssistantComponent));
		// Functions
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAssistantComponent::GetDistanceToPlayer, "{4B3F8176-1A49-49E3-BD26-2AD19D5383DF}"_cry_guid, "GetDistanceToPlayer");
			pFunction->BindOutput(0, 0, "Distance", "Distance");
			componentScope.Register(pFunction);
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAssistantComponent)

CAssistantComponent::~CAssistantComponent()
{
	GameEvents::CPawnEvents::Get()->Unsubscribe(this);
}

void CAssistantComponent::Initialize()
{
	m_pCharacterController = GetEntity()->GetComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	m_pNavigationAgent = GetEntity()->GetComponent<IEntityNavigationComponent>();
	m_pBehaviorTreeComponent = GetEntity()->GetComponent<IEntityBehaviorTreeComponent>();
	m_pAnimationMeshComponent = GetEntity()->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	m_pNavigationAgent->SetStateUpdatedCallback([this](const Vec3& recommendedVelocity)
		{
			m_pCharacterController->ChangeVelocity(recommendedVelocity, Cry::DefaultComponents::CCharacterControllerComponent::EChangeVelocityMode::SetAsTarget);
		});

	m_pNavigationAgent->SetNavigationCompletedCallback([this](bool reached)
		{
		});


	//Look At
	CryCreateClassInstanceForInterface(cryiidof<IAnimationOperatorQueue>(), m_lookAtModifier);
	m_bRandomLook = true;

	//Rotate Engines
	CryCreateClassInstanceForInterface(cryiidof<IAnimationOperatorQueue>(), m_engineRotateAtModifier);	
}

IEntityComponent::ComponentEventPriority CAssistantComponent::GetEventPriority() const
{
	return 16;
}

Cry::Entity::EventFlags CAssistantComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::Update | Cry::Entity::EEvent::GameplayStarted | Cry::Entity::EEvent::Reset;
}

void CAssistantComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Reset:
	{
		if (event.nParam[0])
		{
			if (m_pFlashlightEntity)
			{
				gEnv->pEntitySystem->RemoveEntity(m_pFlashlightEntity->GetId());
				m_pFlashlightEntity = nullptr;
			}
		}
	}
	break;
	case Cry::Entity::EEvent::Update:
	{
		UpdateLook();
		UpdateEngineDir();
		UpdateCheckCurrentPosition();
	}
	break;
	case Cry::Entity::EEvent::GameplayStarted:
	{
		GameEvents::CPawnEvents::Get()->Subscribe(this);

		m_playerEntity = CPlayerController::Get()->GetControlledPawn()->GetEntity();
		p_DRSActor = gEnv->pDynamicResponseSystem->CreateResponseActor("Assistant", GetEntityId());

		InitializeEngineBones();
		CreateFlashlight();
	}
	break;
	}
}


void CAssistantComponent::SendEventBehaviorTree(string signalName)
{
	m_pBehaviorTreeComponent->SendEvent(signalName.c_str());
	CryLog(signalName);
}

bool CAssistantComponent::MoveToPlayer()
{
	if(m_pCharacterController)
	{
		if (m_playerEntity)
		{
			Vec3 playerPos = m_playerEntity->GetWorldPos();

			if (m_pNavigationAgent->IsDestinationReachable(playerPos) == false)
			{
				return true;
			}

			m_pNavigationAgent->NavigateTo(playerPos);

			Quat rot = Quat::CreateRotationVDir(m_pCharacterController->GetVelocity());
			Quat lastRot = m_pEntity->GetRotation();
			Quat qResult = Quat::CreateSlerp(lastRot, rot, gEnv->pTimer->GetFrameTime() * 7.f);

			GetEntity()->SetRotation(qResult);

			if (GetEntity()->GetWorldPos().GetDistance(playerPos) <= 3.5f)
			{
				return true;
			}
		}
	}

	return false;
}

bool CAssistantComponent::FollowToPlayer()
{
	return false;
}

bool CAssistantComponent::Searching(Vec3 searchPos)
{
	if (m_pCharacterController)
	{
		if (m_playerEntity)
		{
			if (m_pNavigationAgent->IsDestinationReachable(searchPos) == false)
			{
				for (int i = 0; i < 3; i++)
				{
					searchPos = GetEntity()->GetWorldPos() + Vec3(cry_random(-10.0f, 10.0f), cry_random(-10.0f, 10.0f), 0);
					if (m_pNavigationAgent->IsDestinationReachable(searchPos))
					{
						continue;
					}
				}

				return true;
			}
			m_pNavigationAgent->NavigateTo(searchPos);

			Quat rot = Quat::CreateRotationVDir(m_pCharacterController->GetVelocity());
			Quat lastRot = m_pEntity->GetRotation();
			Quat qResult = Quat::CreateSlerp(lastRot, rot, gEnv->pTimer->GetFrameTime() * 7.f);

			GetEntity()->SetRotation(qResult);

			if (GetEntity()->GetWorldPos().GetDistance(searchPos) <= 1.5f)
			{
				return true;
			}
		}
	}

	return false;
}

bool CAssistantComponent::MoveToLocation(Vec3 targetPos)
{
	if (m_pCharacterController)
	{
		if (m_pNavigationAgent->IsDestinationReachable(targetPos) == false)
		{
			return true;
		}

		m_pNavigationAgent->NavigateTo(targetPos);

		Quat rot = Quat::CreateRotationVDir(m_pCharacterController->GetVelocity());
		Quat lastRot = m_pEntity->GetRotation();
		Quat qResult = Quat::CreateSlerp(lastRot, rot, gEnv->pTimer->GetFrameTime() * 7.f);

		GetEntity()->SetRotation(qResult);

		if (GetEntity()->GetWorldPos().GetDistance(targetPos) <= 1.5f)
		{
			return true;
		}
	}

	return false;
}

Vec3 CAssistantComponent::FindReachablePosition(Vec3 position)
{
	//raycast
	const auto rayFlags = rwi_stop_at_pierceable | rwi_colltype_any;
	ray_hit hit;
	const bool bRayHit = gEnv->pPhysicalWorld->RayWorldIntersection(position, Vec3(0, 0, -1) * gEnv->p3DEngine->GetMaxViewDistance(), ent_all, rayFlags, &hit, 1);

	if (bRayHit)
	{
		if (m_pNavigationAgent->IsDestinationReachable(hit.pt))
		{
			return hit.pt;
		}

		Vec3 lPos = position;
		for (int i = 0; i < 10; i++)
		{			
			const bool bRayHit2 = gEnv->pPhysicalWorld->RayWorldIntersection(lPos, Vec3(0,0,-1) * gEnv->p3DEngine->GetMaxViewDistance(), ent_all, rayFlags, &hit, 1);

			if (bRayHit2)
			{
				if (m_pNavigationAgent->IsDestinationReachable(hit.pt))
				{
					return hit.pt;
				}
				else
				{
					Vec3 dir = (position - m_playerEntity->GetWorldPos()).GetNormalized();
					lPos -= dir * (float)i;
					IPersistantDebug* pDebug = gEnv->pGameFramework->GetIPersistantDebug();
					pDebug->Begin("CHPlayerDebug", false);
					pDebug->AddSphere(lPos, 1.f, ColorF(0.2f, 0.7f, 0.2f), 5.0f);
				}
			}
		}
	}

	return GetEntity()->GetWorldPos();
}

void CAssistantComponent::SendDRSSignal(string signalName)
{
	p_DRSActor->QueueSignal(signalName);
}

float CAssistantComponent::GetDistanceToPlayer()
{
	if (m_playerEntity)
	{
		return GetEntity()->GetWorldPos().GetDistance(m_playerEntity->GetWorldPos());
	}
	return 0.0f;
}

void CAssistantComponent::UpdateLook()
{
	if (ICharacterInstance* pCharacterInstance = m_pAnimationMeshComponent->GetCharacter())
	{
		ISkeletonPose* pSkeleton = pCharacterInstance->GetISkeletonPose();
		IDefaultSkeleton& rIDefaultSkeleton = pCharacterInstance->GetIDefaultSkeleton();

		int m_lookTargetBoneId = rIDefaultSkeleton.GetJointIDByName("LookTarget");

		QuatT lookTargetPos = pSkeleton->GetAbsJointByID(m_lookTargetBoneId);

		Vec3 playerLookPos = Vec3(0, 0, 1.8);

		Quat cameraRot = Quat::CreateRotationVDir(m_pEntity->GetForwardDir());


		//IPersistantDebug* debug = gEnv->pGameFramework->GetIPersistantDebug();
		//debug->Begin("player_lookAT", true);
		//debug->Add2DText(string().Format("angle: %f", RAD2DEG(localCamRotate.GetRotZ())).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());


		Vec3 targetPos = playerLookPos + (cameraRot * (FORWARD_DIRECTION * 5.0)) * m_pEntity->GetRotation() * RandomLook();

		targetPos = Vec3::CreateLerp(lookTargetPos.t, targetPos, gEnv->pTimer->GetFrameTime() * 30.0f);


		IAnimationPoseModifierPtr modPtr = m_lookAtModifier;
		m_pAnimationMeshComponent->GetCharacter()->GetISkeletonAnim()->PushPoseModifier(2, modPtr, "Look Position");

		m_lookAtModifier->PushPosition(m_lookTargetBoneId, IAnimationOperatorQueue::eOp_Override, targetPos);
	}
}

void CAssistantComponent::InitializeEngineBones()
{
	if (ICharacterInstance* pCharacterInstance = m_pAnimationMeshComponent->GetCharacter())
	{
		ISkeletonPose* pSkeleton = pCharacterInstance->GetISkeletonPose();
		IDefaultSkeleton& rIDefaultSkeleton = pCharacterInstance->GetIDefaultSkeleton();

		m_engineRightBoneId = rIDefaultSkeleton.GetJointIDByName("Engine_RightLookTarget");
		m_engineLeftBoneId = rIDefaultSkeleton.GetJointIDByName("Engine_LeftLookTarget");

		m_originalRightEnginePos = pSkeleton->GetAbsJointByID(m_engineRightBoneId).t;
		m_originalLeftEnginePos = pSkeleton->GetAbsJointByID(m_engineLeftBoneId).t;
	}

}

void CAssistantComponent::UpdateEngineDir()
{
	if (m_pCharacterController)
	{
		Vec3 characterVelocity = m_pCharacterController->GetVelocity();
		Quat entityRot = Quat::CreateRotationVDir(m_pEntity->GetForwardDir());

		Vec3 targetRightPos = m_originalRightEnginePos + Vec3(0, m_pEntity->GetForwardDir().len() * -2.0f, 0) * characterVelocity.len();
		targetRightPos = Vec3::CreateLerp(m_originalRightEnginePos, targetRightPos, gEnv->pTimer->GetFrameTime() * 30.0f);

		Vec3 targetLeftPos = m_originalLeftEnginePos + Vec3(0, m_pEntity->GetForwardDir().len() * -2.0f, 0) * characterVelocity.len();
		targetLeftPos = Vec3::CreateLerp(m_originalLeftEnginePos, targetLeftPos, gEnv->pTimer->GetFrameTime() * 30.0f);

		IAnimationPoseModifierPtr modPtr = m_engineRotateAtModifier;
		m_pAnimationMeshComponent->GetCharacter()->GetISkeletonAnim()->PushPoseModifier(3, modPtr, "Engine Rotation");


		m_engineRotateAtModifier->PushPosition(m_engineRightBoneId, IAnimationOperatorQueue::eOp_Override, targetRightPos);
		m_engineRotateAtModifier->PushPosition(m_engineLeftBoneId, IAnimationOperatorQueue::eOp_Override, targetLeftPos);
	}
}

void CAssistantComponent::UpdateCheckCurrentPosition()
{
	if (m_pNavigationAgent->IsDestinationReachable(GetEntity()->GetWorldPos()) == true)
	{
		m_lastReachablePosition = GetEntity()->GetWorldPos() - GetEntity()->GetForwardDir() * 2.0f;
	}
	else
	{
		m_pNavigationAgent->NavigateTo(m_lastReachablePosition);
		GetEntity()->SetPos(m_lastReachablePosition);
	}
}

Quat CAssistantComponent::RandomLook()
{
	if (m_bRandomLook)
	{
		if (m_currentGlobalLookTime > 0)
		{
			m_currentGlobalLookTime -= gEnv->pTimer->GetFrameTime();

			if (m_currentLocalLookTime > 0)
			{
				m_currentLocalLookTime -= gEnv->pTimer->GetFrameTime();
			}
			else
			{
				// set local target
				m_currentRandomLookRotation *= Quat::CreateRotationXYZ(Ang3(cry_random(DEG2RAD(-2.0f), DEG2RAD(5.0f)), 0, cry_random(DEG2RAD(-3.0f), DEG2RAD(3.0f))));

				m_currentLocalLookTime = cry_random(m_localLookDuration, m_localLookDuration + 0.5f);
			}
		}
		else
		{
			// set global target
			m_currentRandomLookRotation = Quat::CreateRotationXYZ(Ang3(cry_random(DEG2RAD(-15.0f), DEG2RAD(25.0f)), 0, cry_random(DEG2RAD(-30.0f), DEG2RAD(30.0f))));

			m_currentGlobalLookTime = cry_random(m_globalLookDuration, m_globalLookDuration + 1);
		}
	}

	return m_currentRandomLookRotation;
}

//Create flashlight and attach to bone
void CAssistantComponent::CreateFlashlight()
{
	SEntitySpawnParams flashLightSpawnParams;

	flashLightSpawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(m_flashlightClass.value);
	flashLightSpawnParams.vPosition = Vec3(0, 0, 0);

	m_pFlashlightEntity = gEnv->pEntitySystem->SpawnEntity(flashLightSpawnParams);

	if (ICharacterInstance* pCharacter = m_pAnimationMeshComponent->GetCharacter())
	{
		if (IAttachment* pAttachment = pCharacter->GetIAttachmentManager()->GetInterfaceByName("flashlight"))
		{
			CEntityAttachment* pFlashLightAttachmentObject = new CEntityAttachment();
			pFlashLightAttachmentObject->SetEntityId(m_pFlashlightEntity->GetId());
			pAttachment->AddBinding(pFlashLightAttachmentObject);
		}
	}
}

void CAssistantComponent::OnStartSelectTargetForAssistant()
{
	CryLog("START SELECT");
}
