//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"
#include "GamePlugin.h"
#include "PlayerController.h"
#include "Pawns/Alex.h"
#include "Components/SpawnPoint.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CryCore/StaticInstanceList.h>

static void RegisterPlayerControllerComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerController));
		//Functions
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CPlayerController::AlexSpawn, "{D0310FD7-788D-460C-95AD-A4C514515A58}"_cry_guid, "AlexSpawn");
			componentScope.Register(pFunction);
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerControllerComponent);

CPlayerController* CPlayerController::m_pInstance = nullptr;

void CPlayerController::Initialize()
{
	if (m_pInstance)
	{
		if (m_pInstance->m_pEntity == m_pEntity)
			return;

		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "More than one player controller is not allowed. Player controller removed from entity '%s'.", m_pInstance->m_pEntity->GetName());
		m_pInstance->m_pEntity->RemoveComponent<CPlayerController>();
	}
	m_pInstance = this;

	CPlayerInput::GetInput()->RegisterActionMaps(GetEntity());

	if (!gEnv->IsEditor())
	{
		AlexSpawn();
		AssistantSpawn();
	}

	m_pUIManager = new CUIManager();
}

Cry::Entity::EventFlags CPlayerController::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted | Cry::Entity::EEvent::Reset;
}

void CPlayerController::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted:
	{
		const Matrix34 newTransform = CSpawnPointComponent::GetFirstSpawnPointTransform();
		GetControlledPawn()->GetEntity()->SetWorldTM(newTransform);		
	}
	break;

	case Cry::Entity::EEvent::Reset:
	{
		if (event.nParam[0] == 1)
		{
			AlexSpawn();
			AssistantSpawn();
		}

		if (event.nParam[0] == 0)
		{
			m_pControlledPawn = nullptr;
			m_pAssistant = nullptr;
			//gEnv->pEntitySystem->RemoveEntity(m_pControlledPawn->GetEntityId(), true);
			//m_pControlledPawn = nullptr;
		}
	}
	break;

	}
}

void CPlayerController::AlexSpawn()
{
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(m_alexPlayerClass.value);
	const string playerName = "AlexPlayer";
	spawnParams.sName = playerName;

	spawnParams.nFlags |= ENTITY_FLAG_LOCAL_PLAYER | ENTITY_FLAG_TRIGGER_AREAS;

	spawnParams.vPosition = m_pEntity->GetWorldPos();
	spawnParams.qRotation = m_pEntity->GetWorldRotation();

	if (IEntity* pPlayerEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams))
	{
		const Matrix34 newTransform = CSpawnPointComponent::GetFirstSpawnPointTransform();
		pPlayerEntity->SetWorldTM(newTransform);
		CSpawnPointComponent::RemoveSpawnPoint();

		CAlexPlayer* pPlayer = pPlayerEntity->GetComponent<CAlexPlayer>();
		SetControlledPawn(pPlayer);
	}
}

void CPlayerController::AssistantSpawn()
{
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(m_assistantClass.value);
	const string playerName = string().Format("AssistantJoe %", PRISIZE_T);
	spawnParams.sName = playerName;

	spawnParams.nFlags |= ENTITY_FLAG_TRIGGER_AREAS;

	spawnParams.vPosition = m_pEntity->GetWorldPos();
	spawnParams.qRotation = m_pEntity->GetWorldRotation();

	if (IEntity* pAssistantEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams))
	{
		const Matrix34 newTransform = CSpawnPointComponent::GetFirstSpawnPointTransform();
		pAssistantEntity->SetWorldTM(newTransform);
		CSpawnPointComponent::RemoveSpawnPoint();

		m_pAssistant = pAssistantEntity;
	}
}
