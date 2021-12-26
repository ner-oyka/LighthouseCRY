//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"
#include "Alex.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>
#include <CrySchematyc/IObject.h>
#include <CryCore/StaticInstanceList.h>
#include "Framework/PlayerController.h"
#include "Framework/CameraController.h"
#include "Components/Assistant.h"

#include <CryPhysics\physinterface.h>
#include <CryPhysics/IPhysics.h>

#include <CryInput/IHardwareMouse.h>

#include <CryGame/GameUtils.h>
#include <CryMath/Random.h>

#include "Car.h"

#include "Framework/Events/PawnEvents.h"

// REGISTER SIGNALS
void ReflectType(Schematyc::CTypeDesc<SSetDrivingSignal>& desc)
{
	desc.SetGUID("{CA46E001-1171-414B-A6E5-B114052CC01A}"_cry_guid);
	desc.SetLabel("OnSetDriving");
}

void ReflectType(Schematyc::CTypeDesc<SReleaseDrivingSignal>& desc)
{
	desc.SetGUID("{89816BE9-63D9-4466-92CD-D0657DB3C11D}"_cry_guid);
	desc.SetLabel("OnReleaseDriving");
}

void ReflectType(Schematyc::CTypeDesc<SStartResearchTargetingSignal>& desc)
{
	desc.SetGUID("{651EADE6-C679-4C70-BE0A-F6E6D63BF220}"_cry_guid);
	desc.SetLabel("OnStartResearchTargeting");
}

void ReflectType(Schematyc::CTypeDesc<SStopResearchTargetingSignal>& desc)
{
	desc.SetGUID("{848B9A7B-79CA-4D4A-A72B-73BF16DE154B}"_cry_guid);
	desc.SetLabel("OnStopResearchTargeting");
}

void ReflectType(Schematyc::CTypeDesc<SDeathSignal>& desc)
{
	desc.SetGUID("{00D15660-8129-41DD-A160-D309C029A6ED}"_cry_guid);
	desc.SetLabel("OnDeath");
}

void ReflectType(Schematyc::CTypeDesc<SStartFightSignal>& desc)
{
	desc.SetGUID("{F03EF724-A25B-409F-A341-00878C9E41C1}"_cry_guid);
	desc.SetLabel("OnStartFight");
}

void ReflectType(Schematyc::CTypeDesc<SReleaseFightSignal>& desc)
{
	desc.SetGUID("{36474E34-58C5-4068-874E-257AB89B340E}"_cry_guid);
	desc.SetLabel("OnReleaseFight");
}

void ReflectType(Schematyc::CTypeDesc<SStartFire>& desc)
{
	desc.SetGUID("{4C3A4C61-D5E6-4DAD-8750-91E4E1FCA462}"_cry_guid);
	desc.SetLabel("OnStartFire");
}

void ReflectType(Schematyc::CTypeDesc<SStopFire>& desc)
{
	desc.SetGUID("{34B7CD24-5D8E-4FAF-AFA0-7193DCBCF1CC}"_cry_guid);
	desc.SetLabel("OnStopFire");
}

// REGISTER COMPONENT
static void RegisterAlexPlayer(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CAlexPlayer));
		//Functions
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAlexPlayer::UpdateMovement, "{A94A6AC8-86B3-4E7A-81DD-E33C42BB67DF}"_cry_guid, "UpdateMovement");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAlexPlayer::UpdateAnimation, "{6FDBE24A-1CB5-47BD-AA67-8B9AEA9600FB}"_cry_guid, "UpdateAnimation");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAlexPlayer::UpdateCursor, "{9CB0417D-4F86-44BC-9B33-E583D95A0435}"_cry_guid, "UpdateCursor");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAlexPlayer::SpawnCursorEntity, "{BC385126-F4E7-47E0-A662-6D59A897C061}"_cry_guid, "SpawnCursor");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAlexPlayer::RemoveCursorEntity, "{A8FC5B04-CFB6-4449-8743-3CBD88D2C946}"_cry_guid, "RemoveCursor");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAlexPlayer::UpdateFightMovement, "{415E498E-C714-4640-A3D9-D2673DC830CE}"_cry_guid, "UpdateFightMovement");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAlexPlayer::PhysicalizeDeath, "{592F9C3D-612D-4352-AAC4-F9A788561B47}"_cry_guid, "PhysicalizeDeath");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAlexPlayer::CheckCover, "{83CB6FCB-06E7-4C3E-BEE2-4E7E3E44F507}"_cry_guid, "CheckCover");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAlexPlayer::UpdateLookAt, "{1D05E914-AF31-4430-96A6-0D1EF3E01981}"_cry_guid, "UpdateLookAt");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAlexPlayer::EnableWeaponAim, "{61A40258-8AF9-4D4B-971A-04803C434D00}"_cry_guid, "EnableWeaponAim");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAlexPlayer::DisableWeaponAim, "{6D186412-F43A-4AD9-82F3-75CEF8D7162D}"_cry_guid, "DisableWeaponAim");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CAlexPlayer::UpdateWeaponAim, "{5BB9C457-9F70-40EB-B5C5-4D4DF32B2B0E}"_cry_guid, "UpdateWeaponAim");
			componentScope.Register(pFunction);
		}

		//Signals
		{
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(SSetDrivingSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(SReleaseDrivingSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(SStartResearchTargetingSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(SStopResearchTargetingSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(SDeathSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(SStartFightSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(SReleaseFightSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(SStartFire));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(SStopFire));
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAlexPlayer);

CAlexPlayer::~CAlexPlayer()
{
	ReleaseInputs();
}

void CAlexPlayer::Initialize()
{
	m_pCharacterController = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));

	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	//m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/Dude.adb");
	//m_pAnimationComponent->SetCharacterFile("Objects/Characters/Dude/dude.cdf");

	//m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/DudeDefenition.xml");
	//m_pAnimationComponent->SetDefaultScopeContextName("DudeCharacter");

	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/Astra.adb");
	m_pAnimationComponent->SetCharacterFile("Objects/Characters/Astra/astra.cdf");

	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/AstraDefenition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("AstraCharacter");




	//m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/FirstPerson.adb");
	//m_pAnimationComponent->SetCharacterFile("Objects/Characters/SampleCharacter/thirdperson.cdf");

	//m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	//m_pAnimationComponent->SetDefaultScopeContextName("FirstPersonCharacter");



	m_pAnimationComponent->SetDefaultFragmentName("Idle");

	m_pAnimationComponent->SetAnimationDrivenMotion(true);

	m_pAnimationComponent->EnableGroundAlignment(true);

	m_pAnimationComponent->LoadFromDisk();

	// Acquire fragment and tag identifiers to avoid doing so each update
	m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
	m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
	m_rotateTagId = m_pAnimationComponent->GetTagId("Rotate");
	m_crouchTagId = m_pAnimationComponent->GetTagId("Crouch");
	m_rifleTagId = m_pAnimationComponent->GetTagId("Rifle");

	m_pAnimationComponent->ResetCharacter();


	m_pCharacterController->Physicalize();

	m_speed = m_baseSpeed;
	m_newSpeed = m_speed;

	//Look At
	CryCreateClassInstanceForInterface(cryiidof<IAnimationOperatorQueue>(), m_lookAtModifier);
	m_bRandomLook = true;
}

Cry::Entity::EventFlags CAlexPlayer::GetEventMask() const
{
	return
		Cry::Entity::EEvent::Update | Cry::Entity::EEvent::Reset |
		Cry::Entity::EEvent::PrePhysicsUpdate | Cry::Entity::EEvent::PhysicsCollision |
		Cry::Entity::EEvent::GameplayStarted;
}

void CAlexPlayer::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Reset:
	{
		bool m_isAlive = event.nParam[0] != 0;
		if (m_isAlive)
		{
			//SpawnCursorEntity();
		}
		else
		{
			// Removed by Sandbox
			m_pCursorEntity = nullptr;
			m_pTargetMeshToAssistant = nullptr;
		}
	}
	break;

	case Cry::Entity::EEvent::GameplayStarted:
	{
		if (ICharacterInstance* pCharacterInstance = m_pAnimationComponent->GetCharacter())
		{
			IAnimationPoseBlenderDir* pPoseBlenderAim = m_pAnimationComponent->GetCharacter()->GetISkeletonPose()->GetIPoseBlenderAim();
			if (pPoseBlenderAim)
			{
				if (ISkeletonAnim* skeletonAnim = pCharacterInstance->GetISkeletonAnim())
				{
					CryCharAnimationParams animParams;
					animParams.m_nLayerID = 4;
					animParams.m_nFlags = CA_LOOP_ANIMATION;
					animParams.m_fPlaybackWeight = 1.0f;

					skeletonAnim->StartAnimation("astra_AimPoses", animParams);
					pPoseBlenderAim->SetLayer(4);

					pPoseBlenderAim->SetState(false);
				}
			}
		}
	}
	break;

	case Cry::Entity::EEvent::Update:
	{
		const float frameTime = event.fParam[0];
	}
	break;

	case Cry::Entity::EEvent::PrePhysicsUpdate:
	{

	}
	break;

	case Cry::Entity::EEvent::PhysicsCollision:
	{
		if (const EventPhysCollision* pCollision = reinterpret_cast<const EventPhysCollision*>(event.nParam[0]))
		{
			if (pCollision->idmat[1] == gEnv->pPhysicalWorld->GetWaterMat())
			{
				CryLog("WATER!");
			}
			//if (pCollision->pEntity[(int)!event.nParam[1]]->GetType() == PE_PARTICLE)
			//{
			//	CryLog("WATER!");
			//}

			const Vec3& contactPoint = pCollision->pt;
			//const Vec3& relativeContactVelocity = pCollision->vloc[0];
			const Vec3& relativeContactVelocity2 = pCollision->vloc[1];
			const float contactMass = pCollision->mass[1];

			if (IEntity* pEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pCollision->pEntity[(int)!event.nParam[1]]))
			{
				if (m_pCollisionEntity == pEntity)
					return;

				m_pCollisionEntity = pEntity;

				m_collisionVelocity = relativeContactVelocity2;
				m_collisionWorldPos = contactPoint;
			}

			//temp
			if (relativeContactVelocity2.len2() > 13.0f && contactMass > 45 || relativeContactVelocity2.len2() > 53.0f && contactMass > 3)
			{
				SendSchematycSignal(SDeathSignal());
			}

			//CryLog("impulse: %f, %f, %f", relativeContactVelocity.x, relativeContactVelocity.y, relativeContactVelocity.z);
			//CryLog("impulse2: %f, %f, %f", relativeContactVelocity2.x, relativeContactVelocity2.y, relativeContactVelocity2.z);
			//CryLog("impulse len: %f", relativeContactVelocity.len2());
			//CryLog("impulse 2 len: %f", relativeContactVelocity2.len2());
			//CryLog("mass 2: %f", contactMass);
		}
		//const int thisEntityIndex = static_cast<int>(event.nParam[1]);
		//const Vec3& contactPoint = pPhysCollision->pt;
		//const Vec3& contactNormal = pPhysCollision->n;
		//const Vec3& relativeContactVelocity = pPhysCollision->vloc[thisEntityIndex];
		//const float contactMass = pPhysCollision->mass[thisEntityIndex];
		
	}
	break;

	}
}

void CAlexPlayer::EnableWeaponAim()
{
	if (ICharacterInstance* pCharacterInstance = m_pAnimationComponent->GetCharacter())
	{
		IAnimationPoseBlenderDir* pPoseBlenderAim = m_pAnimationComponent->GetCharacter()->GetISkeletonPose()->GetIPoseBlenderAim();
		if (pPoseBlenderAim)
		{
			pPoseBlenderAim->SetState(true);
		}
	}
}

void CAlexPlayer::DisableWeaponAim()
{
	if (ICharacterInstance* pCharacterInstance = m_pAnimationComponent->GetCharacter())
	{
		IAnimationPoseBlenderDir* pPoseBlenderAim = m_pAnimationComponent->GetCharacter()->GetISkeletonPose()->GetIPoseBlenderAim();
		if (pPoseBlenderAim)
		{
			pPoseBlenderAim->SetState(false);
		}
	}
}

void CAlexPlayer::UpdateWeaponAim()
{
	if (ICharacterInstance* pCharacterInstance = m_pAnimationComponent->GetCharacter())
	{
		ISkeletonPose* pSkeleton = pCharacterInstance->GetISkeletonPose();

		const CCamera& systemCamera = gEnv->pSystem->GetViewCamera();


		IAnimationPoseBlenderDir* pPoseBlenderAim = pSkeleton->GetIPoseBlenderAim();
		if (pPoseBlenderAim)
		{
			Vec3 targetPos = ZERO;

			const auto rayFlags = rwi_stop_at_pierceable | rwi_colltype_any;
			ray_hit rayhit;
			static IPhysicalEntity* pSkipEnts[10];
			pSkipEnts[0] = GetEntity()->GetPhysics();

			const bool bRayHit = gEnv->pPhysicalWorld->RayWorldIntersection(systemCamera.GetPosition(), systemCamera.GetViewdir() * 1000.0f, ent_all, rayFlags, &rayhit, 1, GetEntity()->GetPhysicalEntity());

			if (bRayHit && rayhit.pCollider != nullptr)
			{
				targetPos = rayhit.pt;
			}
			else
			{
				Vec3 playerOffset = GetEntity()->GetWorldPos() + GetEntity()->GetForwardDir() * 10.0f;
				targetPos = systemCamera.GetPosition() + systemCamera.GetViewdir() * playerOffset.GetDistance(systemCamera.GetPosition());
			}

			pPoseBlenderAim->SetTarget(targetPos);

			//IPersistantDebug* pDebug = gEnv->pGameFramework->GetIPersistantDebug();
			//pDebug->Begin("CHPlayerDebug", false);
			//pDebug->AddSphere(targetPos, 1.0f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime() + 0.01f);
		}
	}
}

IEntity* CAlexPlayer::FindCover()
{
	if (ICharacterInstance* pChar = m_pAnimationComponent->GetCharacter())
	{
		if (ISkeletonPose* pSkel = pChar->GetISkeletonPose())
		{
			const QuatT& headPos = pSkel->GetAbsJointByID(pChar->GetIDefaultSkeleton().GetJointIDByName("mixamorig:Head"));
			const QuatT& pelvisPos = pSkel->GetAbsJointByID(pChar->GetIDefaultSkeleton().GetJointIDByName("mixamorig:Hips"));
			QuatT root(m_pEntity->GetWorldTM());

			Vec3 headWorldPos = root * headPos.t;
			Vec3 pelvisWorldPos = root * pelvisPos.t;

			const CCamera& systemCamera = gEnv->pSystem->GetViewCamera();
			Vec3 movementRequest = (GetMovement(Quat(systemCamera.GetMatrix())) + GetXiMovement(Quat(systemCamera.GetMatrix()))).GetNormalized() * m_speed;
			if (movementRequest.len() == 0.0f)
			{
				movementRequest = m_pEntity->GetForwardDir();
			}

			//raycast
			const auto rayFlags = rwi_stop_at_pierceable | rwi_colltype_any;
			ray_hit hit;
			const bool bRayHit = gEnv->pPhysicalWorld->RayWorldIntersection(pelvisWorldPos, m_pEntity->GetForwardDir() * 2.0f, ent_all, rayFlags, &hit, 1, GetEntity()->GetPhysicalEntity());

			IPersistantDebug* pDebug = gEnv->pGameFramework->GetIPersistantDebug();
			pDebug->Begin("CHPlayerDebug", false);
			pDebug->AddDirection(headWorldPos, 1.0f, m_pEntity->GetForwardDir() * 2.0f, ColorF(0.2f, 0.7f, 0.2f), 0.1f);
			pDebug->AddDirection(pelvisWorldPos, 1.0f, m_pEntity->GetForwardDir() * 2.0f, ColorF(0.2f, 0.7f, 0.2f), 0.1f);

			if (bRayHit && hit.pCollider != nullptr)
			{
				IEntity* pHitEntity = static_cast<IEntity*>(hit.pCollider->GetForeignData(PHYS_FOREIGN_ID_ENTITY));
				if (pHitEntity)
				{
					return pHitEntity;
				}
			}
		}
	}
	return nullptr;
}

void CAlexPlayer::CheckCover()
{
	if (currentCoverEntity)
	{
		if (m_pEntity->GetWorldPos().GetDistance(currentCoverEntity->GetWorldPos()) > 2.0f)
		{
			m_pAnimationComponent->SetTagWithId(m_crouchTagId, false);
			CryLog("cover exit");
			currentCoverEntity = nullptr;
		}
	}

	if (IEntity* cover = FindCover())
	{
		if (currentCoverEntity == cover)
		{
			return;
		}
		else
		{
			currentCoverEntity = cover;
			m_pAnimationComponent->SetTagWithId(m_crouchTagId, true);
			CryLog("cover enter");
		}
	}
}

void CAlexPlayer::SpawnCursorEntity()
{
	if (m_pCursorEntity)
	{
		gEnv->pEntitySystem->RemoveEntity(m_pCursorEntity->GetId());
		m_pCursorEntity = nullptr;
	}

	SEntitySpawnParams spawnParams;
	// No need for a special class!
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	spawnParams.vPosition = Vec3(0,0,9999);

	// Spawn the cursor
	m_pCursorEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams);

	// Load geometry
	const int geometrySlot = 0;
	m_pCursorEntity->LoadGeometry(geometrySlot, "%ENGINE%/EngineAssets/Objects/primitive_sphere.cgf");

	// Scale the cursor down a bit
	m_pCursorEntity->SetScale(Vec3(3.0f));
	m_pCursorEntity->SetViewDistRatio(255);

	// Load the custom cursor material
	IMaterial* pCursorMaterial = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/targetCursor");
	m_pCursorEntity->SetMaterial(pCursorMaterial);
}

void CAlexPlayer::RemoveTargetMeshToAssistant()
{
	gEnv->pEntitySystem->RemoveEntity(m_pTargetMeshToAssistant->GetId());
	m_pTargetMeshToAssistant = nullptr;
}

void CAlexPlayer::RemoveCursorEntity()
{
	if (m_pCursorEntity)
	{
		gEnv->pEntitySystem->RemoveEntity(m_pCursorEntity->GetId());
		m_pCursorEntity = nullptr;
	}
}

void CAlexPlayer::UpdateCursor()
{
	mouseX += m_mouseDeltaRotation.x + m_xiLookDelta.x * 25.0f;
	mouseY += m_mouseDeltaRotation.y + m_xiLookDelta.y * 25.0f;

	mouseX = crymath::clamp(mouseX, 0.0f, (float)gEnv->pRenderer->GetWidth());
	mouseY = crymath::clamp(mouseY, 0.0f, (float)gEnv->pRenderer->GetHeight());

	Vec3 vPos0(0, 0, 0);
	gEnv->pRenderer->UnProjectFromScreen(mouseX, mouseY, 0, &vPos0.x, &vPos0.y, &vPos0.z);


	Vec3 vPos1(0, 0, 0);
	gEnv->pRenderer->UnProjectFromScreen(mouseX, mouseY, 1, &vPos1.x, &vPos1.y, &vPos1.z);

	Vec3 vDir = vPos1 - vPos0;
	vDir.Normalize();

	const CCamera& systemCamera = gEnv->pSystem->GetViewCamera();

	const unsigned int rayFlags = rwi_colltype_any | rwi_ignore_noncolliding | rwi_stop_at_pierceable;
	ray_hit hit;

	if (gEnv->pPhysicalWorld->RayWorldIntersection(systemCamera.GetPosition(), systemCamera.GetViewdir() * gEnv->p3DEngine->GetMaxViewDistance(), ent_all, rayFlags, &hit, 1, this->GetEntity()->GetPhysicalEntity()))
	{
		m_cursorPositionInWorld = hit.pt;
	}
	else
	{
		m_cursorPositionInWorld = systemCamera.GetPosition() + systemCamera.GetViewdir() * 100.0f;
	}

	if (m_pCursorEntity)
	{
		m_pCursorEntity->SetPosRotScale(m_cursorPositionInWorld, IDENTITY, m_pCursorEntity->GetScale());
	}
}

Quat CAlexPlayer::RandomLookAt()
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

				m_currentLocalLookTime = cry_random(m_localLookDuration - 0.5f, m_localLookDuration + 0.5f);
			}
		}
		else
		{
			// set global target
			m_currentRandomLookRotation = Quat::CreateRotationXYZ(Ang3(cry_random(DEG2RAD(-15.0f), DEG2RAD(25.0f)), 0, cry_random(DEG2RAD(-30.0f), DEG2RAD(30.0f))));

			m_currentGlobalLookTime = cry_random(m_globalLookDuration - 1, m_globalLookDuration + 3);
		}
	}

	return m_currentRandomLookRotation;
}

template <class T>
void CAlexPlayer::SendSchematycSignal(const T& event)
{
	if (Schematyc::IObject* pSchematycObject = GetEntity()->GetSchematycObject())
	{
		pSchematycObject->ProcessSignal(event, GetGUID());
	}
}

void CAlexPlayer::UpdateAnimation()
{
	Interpolate(m_speed, m_newSpeed, 2, gEnv->pTimer->GetFrameTime());
	m_pAnimationComponent->SetMotionParameter(eMotionParamID_TravelSpeed, m_speed);

	const FragmentID& desiredFragmentId = IsMovementRequested() || m_xiMovementDelta.len() != 0.0f ? m_walkFragmentId : m_idleFragmentId;
	if (m_activeFragmentId != desiredFragmentId)
	{
		m_activeFragmentId = desiredFragmentId;
		m_pAnimationComponent->QueueFragmentWithId(m_activeFragmentId);

		if (m_activeFragmentId == m_idleFragmentId)
		{
			gEnv->pConsole->ExecuteString("a_poseAlignerForceWeightOne 1");
		}
		else
		{
			gEnv->pConsole->ExecuteString("a_poseAlignerForceWeightOne 0");
		}
	}
}

void CAlexPlayer::UpdateLookAt()
{
	if (ICharacterInstance* pCharacterInstance = m_pAnimationComponent->GetCharacter())
	{
		ISkeletonPose* pSkeleton = pCharacterInstance->GetISkeletonPose();
		IDefaultSkeleton& rIDefaultSkeleton = pCharacterInstance->GetIDefaultSkeleton();

		int m_lookTargetBoneId = rIDefaultSkeleton.GetJointIDByName("targetLook");

		QuatT lookTargetPos = pSkeleton->GetAbsJointByID(m_lookTargetBoneId);

		const CCamera& systemCamera = gEnv->pSystem->GetViewCamera();


		Vec3 playerLookPos = Vec3(0,0,1.8);

		Quat cameraRot = Quat::CreateRotationVDir(systemCamera.GetViewdir());


		Quat localCamRotate = Quat::CreateRotationVDir(m_pEntity->GetForwardDir()) / cameraRot;
		float localCamZ = RAD2DEG(localCamRotate.GetRotZ());
		if (localCamZ <= -125.0f || localCamZ >= 125.0f)
		{
			cameraRot = Quat::CreateRotationVDir(m_pEntity->GetForwardDir());
		}

		//IPersistantDebug* debug = gEnv->pGameFramework->GetIPersistantDebug();
		//debug->Begin("player_lookAT", true);
		//debug->Add2DText(string().Format("angle: %f", RAD2DEG(localCamRotate.GetRotZ())).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());

		
		Vec3 targetPos = playerLookPos + (cameraRot * (FORWARD_DIRECTION * 5.0)) * m_pEntity->GetRotation() * RandomLookAt();

		targetPos = Vec3::CreateLerp(lookTargetPos.t, targetPos, gEnv->pTimer->GetFrameTime() * 5);


		IAnimationPoseModifierPtr modPtr = m_lookAtModifier;
		m_pAnimationComponent->GetCharacter()->GetISkeletonAnim()->PushPoseModifier(2, modPtr, "Look Position");

		m_lookAtModifier->PushPosition(m_lookTargetBoneId, IAnimationOperatorQueue::eOp_Override, targetPos);
	}
}

void CAlexPlayer::OnForward(int activationMode, float value)
{
	HandleInputFlagChange((TInputFlags)EInputFlag::Forward, activationMode);
}

void CAlexPlayer::OnBackward(int activationMode, float value)
{
	HandleInputFlagChange((TInputFlags)EInputFlag::Backward, activationMode);
}

void CAlexPlayer::OnLeft(int activationMode, float value)
{
	HandleInputFlagChange((TInputFlags)EInputFlag::Left, activationMode);
}

void CAlexPlayer::OnRight(int activationMode, float value)
{
	HandleInputFlagChange((TInputFlags)EInputFlag::Right, activationMode);
}

void CAlexPlayer::OnShift(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		m_newSpeed = m_sprintSpeed;
	}
	if (activationMode == eAAM_OnRelease)
	{
		m_newSpeed = m_baseSpeed;
	}
}

void CAlexPlayer::OnXI_B(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		m_newSpeed = m_sprintSpeed;
	}
	if (activationMode == eAAM_OnRelease)
	{
		m_newSpeed = m_baseSpeed;
	}
}

void CAlexPlayer::OnMouseX(int activationMode, float value)
{
	m_mouseDeltaRotation.x = value;
}

void CAlexPlayer::OnMouseY(int activationMode, float value)
{
	m_mouseDeltaRotation.y = value * -1.0f;
}

void CAlexPlayer::SpawnMeshTargetForAssistant()
{
	if (m_pTargetMeshToAssistant)
	{
		gEnv->pEntitySystem->RemoveEntity(m_pTargetMeshToAssistant->GetId());
		m_pTargetMeshToAssistant = nullptr;
	}

	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	spawnParams.vPosition = m_cursorPositionInWorld + Vec3(0,0,0.5f);

	m_pTargetMeshToAssistant = gEnv->pEntitySystem->SpawnEntity(spawnParams);

	// Load geometry
	const int geometrySlot = 0;
	m_pTargetMeshToAssistant->LoadGeometry(geometrySlot, "%ENGINE%/EngineAssets/Objects/primitive_sphere.cgf");

	// Scale the cursor down a bit
	m_pTargetMeshToAssistant->SetScale(Vec3(1.0f));
	m_pTargetMeshToAssistant->SetViewDistRatio(255);

	// Load the custom cursor material
	IMaterial* pCursorMaterial = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/targetCursor.mtl");
	m_pTargetMeshToAssistant->SetMaterial(pCursorMaterial);
}

void CAlexPlayer::OnMouseButtonLeft(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		//todo
		SendSchematycSignal(SStartFire());
	}
	if (activationMode == eAAM_OnRelease)
	{
		SendSchematycSignal(SStopFire());
	}
}

void CAlexPlayer::OnF(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (m_isResearchTargeting)
		{
			SpawnMeshTargetForAssistant();
			CPlayerController::Get()->GetAssistant()->GetComponent<CAssistantComponent>()->SendEventBehaviorTree("MoveToSearchLocation");
		}
	}
}

void CAlexPlayer::OnOne(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (!m_isResearchTargeting)
		{
			m_isFight = !m_isFight;
			SetFight(m_isFight);
		}
	}
}

void CAlexPlayer::SetFight(bool value)
{
	if (value)
	{
		//set signal to fight state
		SendSchematycSignal(SStartFightSignal());
		GameEvents::CPawnEvents::Get()->SendEvent(GameEvents::EPawnEvent::Pawn_StartFight);
		m_pAnimationComponent->SetTagWithId(m_rifleTagId, true);
	}
	else
	{
		//send signal to movement state
		SendSchematycSignal(SReleaseFightSignal());
		GameEvents::CPawnEvents::Get()->SendEvent(GameEvents::EPawnEvent::Pawn_ReleaseFight);
		m_pAnimationComponent->SetTagWithId(m_rifleTagId, false);
	}
}

void CAlexPlayer::OnMouseButtonRight(int activationMode, float value)
{
	if (m_isFight == false)
	{
		if (activationMode == eAAM_OnPress)
		{
			SendSchematycSignal(SStartResearchTargetingSignal());
			m_isResearchTargeting = true;
			//SpawnCursorEntity();
			GameEvents::CPawnEvents::Get()->SendEvent(GameEvents::EPawnEvent::Assistant_StartSelectTarget);
		}
		if (activationMode == eAAM_OnRelease)
		{
			SendSchematycSignal(SStopResearchTargetingSignal());
			m_isResearchTargeting = false;
			//RemoveCursorEntity();
			GameEvents::CPawnEvents::Get()->SendEvent(GameEvents::EPawnEvent::Assistant_ReleaseSelectTarget);
		}
	}
}

void CAlexPlayer::OnYawDeltaXILeft(int activationMode, float value)
{
	m_xiMovementDelta.x = -value;
}

void CAlexPlayer::OnPitchDeltaXILeft(int activationMode, float value)
{
	m_xiMovementDelta.y = value;
}

void CAlexPlayer::OnYawDeltaXIRight(int activationMode, float value)
{
	m_xiLookDelta.x = value;
}

void CAlexPlayer::OnPitchDeltaXIRight(int activationMode, float value)
{
	m_xiLookDelta.y = value;
}

void CAlexPlayer::OnEnter(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		CPlayerController::Get()->GetAssistant()->GetComponent<CAssistantComponent>()->SendEventBehaviorTree("ToPlayer");


		//Test car
		IEntity* carEntity = gEnv->pEntitySystem->FindEntityByName("entities::car_test-1");
		if (carEntity)
		{
			SendSchematycSignal(SSetDrivingSignal());

			CCameraControllerComponent::Get()->SetType(Game::ECameraType::Car);
			carEntity->GetComponent<CCarPlayer>()->SetDriver(GetEntity());
			CPlayerController::Get()->SetControlledPawn(carEntity->GetComponent<CCarPlayer>());
		}
	}
}

void CAlexPlayer::OnXI_A(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (m_isResearchTargeting)
		{
			SpawnMeshTargetForAssistant();
			CPlayerController::Get()->GetAssistant()->GetComponent<CAssistantComponent>()->SendEventBehaviorTree("MoveToSearchLocation");
		}
	}
}

void CAlexPlayer::OnTriggerXIRight(int activationMode, float value)
{
	if (value > 0 && m_isResearchTargeting == false)
	{
		SendSchematycSignal(SStartResearchTargetingSignal());
		m_isResearchTargeting = true;
		//SpawnCursorEntity();
		GameEvents::CPawnEvents::Get()->SendEvent(GameEvents::EPawnEvent::Assistant_StartSelectTarget);
	}
	else if (value == 0)
	{
		SendSchematycSignal(SStopResearchTargetingSignal());
		m_isResearchTargeting = false;
		//RemoveCursorEntity();
		GameEvents::CPawnEvents::Get()->SendEvent(GameEvents::EPawnEvent::Assistant_ReleaseSelectTarget);
	}
}

void CAlexPlayer::GetOutTransport()
{
	SendSchematycSignal(SReleaseDrivingSignal());
}

void CAlexPlayer::PhysicalizeDeath()
{
	//m_pEntity->RemoveComponent< Cry::DefaultComponents::CCharacterControllerComponent>();
	//m_pRagdollComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CRagdollComponent>();
	//m_pRagdollComponent->ApplyImpulse(m_collisionVelocity * 5000, m_collisionWorldPos);





	//ICharacterInstance* pCharacter = m_pAnimationComponent->GetCharacter();
	//pCharacter->EnableProceduralFacialAnimation(false);
	//if (gEnv->pSystem->IsSerializingFile() && pCharacter->GetISkeletonPose())
	//	pCharacter->GetISkeletonPose()->SetDefaultPose();

	//SEntityPhysicalizeParams pp;

	//pp.fStiffnessScale = 1.0f;
	//pp.type = PE_ARTICULATED;
	//pp.nSlot = 0;
	//pp.bCopyJointVelocities = true;

	//pp.mass = (float)__fsel(-pp.mass, 80.0f, pp.mass);

	//pe_player_dimensions playerDim;
	//pe_player_dynamics playerDyn;

	//playerDyn.gravity = Vec3(0.f, 0.f, -15.0f);
	//playerDyn.kInertia = 5.5f;

	//pp.pPlayerDynamics = &playerDyn;

	//pp.bCopyJointVelocities = !gEnv->pSystem->IsSerializingFile();
	//pp.nFlagsOR = pef_monitor_poststep;
	//m_pEntity->Physicalize(pp);
}

void CAlexPlayer::UpdateMovement()
{
	if (m_pCharacterController)
	{
		const CCamera& systemCamera = gEnv->pSystem->GetViewCamera();
		Vec3 movementRequest = (GetMovement(Quat(systemCamera.GetMatrix())) + GetXiMovement(Quat(systemCamera.GetMatrix()))).GetNormalized();

		if (movementRequest.len() == 0.0f)
		{
			movementRequest = Vec3(0, 0, 0);
		}
		else
		{
			m_lastMovementRequest = movementRequest;
		}


		if (ICharacterInstance* character = m_pAnimationComponent->GetCharacter())
		{
			ISkeletonAnim& skeletonAnim = *character->GetISkeletonAnim();

			QuatT relMove = skeletonAnim.GetRelMovement();
			Vec3 vel = skeletonAnim.GetCurrentVelocity();

			//test		
			//IPersistantDebug* pDebug = gEnv->pGameFramework->GetIPersistantDebug();
			//pDebug->Begin("CHPlayerDebug", false);
			//pDebug->Add2DText(string().Format("vel: %f, %f, %f", vel.x, vel.y, vel.z).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());
			//pDebug->Add2DText(string().Format("rel: %f, %f, %f", relMove.t.x, relMove.t.y, relMove.t.z).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());
			//pDebug->Add2DText(string().Format("angle: %f", abs(RAD2DEG(relMove.q.GetRotZ()))).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());

			vel = relMove.q * vel;

			m_pCharacterController->SetVelocity(m_pEntity->GetRotation() * vel);
		}


		Quat rot = Quat::CreateRotationVDir(m_lastMovementRequest);
		Quat lastRot = m_pEntity->GetRotation();
		Quat qResult = Quat::CreateSlerp(lastRot, rot, gEnv->pTimer->GetFrameTime() * 7.f);

		GetEntity()->SetRotation(qResult);
	}
}

void CAlexPlayer::UpdateFightMovement()
{
	if (m_pCharacterController)
	{
		const CCamera& systemCamera = gEnv->pSystem->GetViewCamera();
		Vec3 movementRequest = (GetMovement(Quat(systemCamera.GetMatrix())) + GetXiMovement(Quat(systemCamera.GetMatrix()))).GetNormalized();

		if (ICharacterInstance* character = m_pAnimationComponent->GetCharacter())
		{
			ISkeletonAnim& skeletonAnim = *character->GetISkeletonAnim();

			QuatT relMove = skeletonAnim.GetRelMovement();
			Vec3 vel = skeletonAnim.GetCurrentVelocity();

			vel = movementRequest;

			m_pCharacterController->SetVelocity(vel * 2.5f);
		}


		Quat rot = Quat::CreateRotationVDir(systemCamera.GetViewdir());
		rot.v.x = 0;
		rot.v.y = 0;

		Quat lastRot = m_pEntity->GetRotation();
		Quat qResult = Quat::CreateSlerp(lastRot, rot, gEnv->pTimer->GetFrameTime() * 15.f);

		GetEntity()->SetRotation(qResult);
	}
}

Vec3 CAlexPlayer::GetMovement(const Quat& baseRotation)
{
	bool allowMovement = true;
	Quat quatRelativeDirection;
	Vec3 vecMovement = Vec3(ZERO);

	switch (m_inputFlags)
	{
	case (TInputFlags)EInputFlag::Forward:
		quatRelativeDirection = Quat::CreateIdentity();
		break;

	case ((TInputFlags)EInputFlag::Forward | (TInputFlags)EInputFlag::Right):
		quatRelativeDirection = Quat::CreateRotationZ(DEG2RAD(45.0f));
		break;

	case (TInputFlags)EInputFlag::Right:
		quatRelativeDirection = Quat::CreateRotationZ(DEG2RAD(90.0f));
		break;

	case ((TInputFlags)EInputFlag::Backward | (TInputFlags)EInputFlag::Right):
		quatRelativeDirection = Quat::CreateRotationZ(DEG2RAD(135.0f));
		break;

	case (TInputFlags)EInputFlag::Backward:
		quatRelativeDirection = Quat::CreateRotationZ(DEG2RAD(180.0f));
		break;

	case ((TInputFlags)EInputFlag::Backward | (TInputFlags)EInputFlag::Left):
		quatRelativeDirection = Quat::CreateRotationZ(DEG2RAD(225.0f));
		break;

	case (TInputFlags)EInputFlag::Left:
		quatRelativeDirection = Quat::CreateRotationZ(DEG2RAD(270.0f));
		break;

	case ((TInputFlags)EInputFlag::Forward | (TInputFlags)EInputFlag::Left):
		quatRelativeDirection = Quat::CreateRotationZ(DEG2RAD(315.0f));
		break;

	default:
		quatRelativeDirection = Quat::CreateIdentity();
		allowMovement = false;
		break;
	}

	if (allowMovement)
		vecMovement = Vec3(baseRotation.GetFwdX(), baseRotation.GetFwdY(), 0.0f).GetNormalized() * quatRelativeDirection;

	return vecMovement;
}

Vec3 CAlexPlayer::GetXiMovement(const Quat& baseRotation)
{
	if (m_xiMovementDelta.len() != 0.0f)
	{
		return Vec3(baseRotation.GetFwdX(), baseRotation.GetFwdY(), 0.0f).GetNormalized() * Quat::CreateRotationVDir(m_xiMovementDelta);
	}
	return ZERO;
}

