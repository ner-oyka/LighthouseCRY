#include "StdAfx.h"
#include "Explosion.h"

#include <CryPhysics/physinterface.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>

static void RegisterExplosionComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CExplosionComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterExplosionComponent)

void CExplosionComponent::Initialize()
{
	Simulate();
}

void CExplosionComponent::Simulate()
{
	pe_explosion explosion;
	explosion.epicenter = explosion.epicenterImp = GetWorldTransformMatrix().GetTranslation();
	explosion.rmin = 0.2f;
	explosion.rmax = explosion.r = m_radius;
	explosion.forceDeformEntities = true;
	explosion.impulsivePressureAtR = m_impulse;
	explosion.explDir = m_direction;
	
	gEnv->pPhysicalWorld->SimulateExplosion(&explosion);

	CryLog("Explosion Initialize");
}
