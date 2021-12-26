#include "StdAfx.h"
#include "Turret.h"

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>

static void RegisterTurretComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CTurretComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterTurretComponent)

CTurretComponent::~CTurretComponent()
{
}

void CTurretComponent::Initialize()
{
}

Cry::Entity::EventFlags CTurretComponent::GetEventMask() const
{
	return Cry::Entity::EventFlags();
}

void CTurretComponent::ProcessEvent(const SEntityEvent& event)
{
}

void CTurretComponent::Shot()
{
}
