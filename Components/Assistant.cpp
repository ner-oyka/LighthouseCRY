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

void CAssistantComponent::Initialize()
{
	m_pCharacterController = GetEntity()->GetComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	m_pStaticMesh = GetEntity()->GetComponent<Cry::DefaultComponents::CStaticMeshComponent>();
	m_pNavigationAgent = GetEntity()->GetComponent<IEntityNavigationComponent>();
	m_pBehaviorTreeComponent = GetEntity()->GetComponent<IEntityBehaviorTreeComponent>();

	m_pNavigationAgent->SetStateUpdatedCallback([this](const Vec3& recommendedVelocity)
		{
			m_pCharacterController->ChangeVelocity(recommendedVelocity, Cry::DefaultComponents::CCharacterControllerComponent::EChangeVelocityMode::SetAsTarget);
		});

	m_pNavigationAgent->SetNavigationCompletedCallback([this](bool reached)
		{
		});
}

IEntityComponent::ComponentEventPriority CAssistantComponent::GetEventPriority() const
{
	return 16;
}

Cry::Entity::EventFlags CAssistantComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::Update | Cry::Entity::EEvent::GameplayStarted;
}

void CAssistantComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Update:
	{
	}
	break;
	case Cry::Entity::EEvent::GameplayStarted:
	{
		m_playerEntity = CPlayerController::Get()->GetControlledPawn()->GetEntity();
		p_DRSActor = gEnv->pDynamicResponseSystem->CreateResponseActor("Assistant", GetEntityId());

		Matrix34 mat = Matrix34::Create(Vec3(1), Quat::CreateIdentity(), Vec3(0, 0, 3));
		m_pStaticMesh->SetTransformMatrix(mat);
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
