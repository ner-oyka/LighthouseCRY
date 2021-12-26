#include "StdAfx.h"
#include "WeaponAttachment.h"

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

static void RegisterWeaponsAttachment(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CWeaponAttachementComponent));
		// Functions
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CWeaponAttachementComponent::StartShoot, "{6B799F28-0B80-4C8C-B91A-BCDAB99A71D2}"_cry_guid, "StartShoot");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CWeaponAttachementComponent::StopShoot, "{15B9EC58-C164-4501-B015-5816A0101B65}"_cry_guid, "StopShoot");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CWeaponAttachementComponent::SpawnWeaponEntity, "{425FD5F7-556C-4B9E-AE9E-84A96F49728C}"_cry_guid, "SpawnWeaponEntity");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CWeaponAttachementComponent::RemoveCurrentWeapon, "{0ED8DFA7-CEF0-463E-A800-7AF4AFE3C65E}"_cry_guid, "RemoveWeaponEntity");
			componentScope.Register(pFunction);
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWeaponsAttachment)

CWeaponAttachementComponent::~CWeaponAttachementComponent()
{
	RemoveCurrentWeapon();
}

void CWeaponAttachementComponent::Initialize()
{
	//SpawnWeaponEntity();
}

EntityEventMask CWeaponAttachementComponent::GetEventMask() const
{
	return EEntityEvent::Reset;
}

void CWeaponAttachementComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{

	case EEntityEvent::Reset:
	{
		const bool isSimulating = event.nParam[0] > 0;

	} break;

	}
}

void CWeaponAttachementComponent::RemoveCurrentWeapon()
{
	if (m_weaponEntityId != INVALID_ENTITYID)
	{
		gEnv->pEntitySystem->RemoveEntity(m_weaponEntityId, true);
		m_weaponEntityId = INVALID_ENTITYID;
	}
}

void CWeaponAttachementComponent::StartShoot()
{
	IEntity* weaponEntity = gEnv->pEntitySystem->GetEntity(m_weaponEntityId);
	if (CWeaponComponent* weaponComponent = weaponEntity->GetComponent<CWeaponComponent>())
	{
		weaponComponent->StartShootEvent();
	}
}

void CWeaponAttachementComponent::StopShoot()
{
	IEntity* weaponEntity = gEnv->pEntitySystem->GetEntity(m_weaponEntityId);
	if (CWeaponComponent* weaponComponent = weaponEntity->GetComponent<CWeaponComponent>())
	{
		weaponComponent->StopShootEvent();
	}
}

void CWeaponAttachementComponent::SpawnWeaponEntity()
{
	// Make sure we remove any previous weapon that was already spawned
	RemoveCurrentWeapon();

	// Create the Weapon entity attached to the player
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(m_currentWeapon.value);
	spawnParams.sName = nullptr;
	spawnParams.vPosition = ZERO;

	if (IEntity* pWeaponEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams))
	{
		m_weaponEntityId = pWeaponEntity->GetId();

		if (Cry::DefaultComponents::CAdvancedAnimationComponent* animComponent = GetEntity()->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>())
		{
			if (ICharacterInstance* pCharacter = animComponent->GetCharacter())
			{
				if (IAttachment* pAttachment = pCharacter->GetIAttachmentManager()->GetInterfaceByName("Weapon"))
				{
					CEntityAttachment* pWeaponAttachmentObject = new CEntityAttachment();
					pWeaponAttachmentObject->SetEntityId(m_weaponEntityId);
					pAttachment->AddBinding(pWeaponAttachmentObject);
				}
			}
		}
	}
}