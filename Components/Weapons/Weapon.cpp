#include "StdAfx.h"
#include "Weapon.h"

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CrySchematyc/IObject.h>

static void ReflectType(Schematyc::CTypeDesc<CWeaponComponent::SStartShotSignal>& desc)
{
	desc.SetGUID("{1EB8E689-329A-4BCB-BB38-065AF19A4949}"_cry_guid);
	desc.SetLabel("OnStartShoot");
}

static void ReflectType(Schematyc::CTypeDesc<CWeaponComponent::SStopShotSignal>& desc)
{
	desc.SetGUID("{E9F4ECBB-89E7-4CA9-BE9D-4268505D94E2}"_cry_guid);
	desc.SetLabel("OnStopShoot");
}

static void RegisterWeaponComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CWeaponComponent));
		//Functions
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CWeaponComponent::SpawnProjectile, "{30E75B35-1E4A-4903-B20F-4064791A67C5}"_cry_guid, "SpawnProjectile");
			componentScope.Register(pFunction);
		}

		// Signals
		{
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(CWeaponComponent::SStartShotSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(CWeaponComponent::SStopShotSignal));
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWeaponComponent)

CWeaponComponent::~CWeaponComponent()
{
}

void CWeaponComponent::StartShootEvent()
{
	if (Schematyc::IObject* pSchematycObject = m_pEntity->GetSchematycObject())
	{
		pSchematycObject->ProcessSignal(SStartShotSignal(), GetGUID());
	}
}

void CWeaponComponent::StopShootEvent()
{
	if (Schematyc::IObject* pSchematycObject = m_pEntity->GetSchematycObject())
	{
		pSchematycObject->ProcessSignal(SStopShotSignal(), GetGUID());
	}
}

void CWeaponComponent::Initialize()
{
	m_weaponsEnabled = m_startEnabled;

	// Calculate frametime necessary to pass between each projectile
	m_timeBetweenProjectiles = 1 / m_fireSpeed;

	// Fire immediately
	m_timeSinceLastProjectile = m_timeBetweenProjectiles;
}

// Control update order (Lower = Sooner, Default is 14).
IEntityComponent::ComponentEventPriority CWeaponComponent::GetEventPriority() const
{
	// Here we make sure we update after the Player Controller as we want to fire the projectile after
	// rotating the player model rather than before.
	return 16;
}

EntityEventMask CWeaponComponent::GetEventMask() const
{
	return EEntityEvent::Update;
}

void CWeaponComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{

	case EEntityEvent::Update:
	{
		if (!gEnv->IsEditing())
		{
			SEntityUpdateContext* pContext = (SEntityUpdateContext*)event.nParam[0];

			m_timeSinceLastProjectile += pContext->fFrameTime;

			if (m_timeSinceLastProjectile >= m_timeBetweenProjectiles)
			{
				if (m_weaponsEnabled)
				{
					// Keep remainder
					m_timeSinceLastProjectile = m_timeSinceLastProjectile - m_timeBetweenProjectiles;
				}
				else
				{
					m_timeSinceLastProjectile = m_timeBetweenProjectiles;
				}
			}
		}

	} break;

	}
}

void CWeaponComponent::SpawnProjectile()
{
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(m_projectileClassname.value);
	spawnParams.sName = nullptr;
	spawnParams.vPosition = GetEntity()->GetWorldPos() - GetEntity()->GetForwardDir() * 0.7f + Vec3(0,0,0.1f); //test
	spawnParams.qRotation = GetEntity()->GetRotation();

	gEnv->pEntitySystem->SpawnEntity(spawnParams);
}
