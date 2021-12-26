#include "StdAfx.h"
#include "Projectile.h"

#include <DefaultComponents/Physics/RigidBodyComponent.h>

#include <CryPhysics/physinterface.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>

#include "Framework/PlayerController.h"

#include <CryMath/Random.h>

static void RegisterProjectileComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CProjectileComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterProjectileComponent)

CProjectileComponent::~CProjectileComponent()
{
}

void CProjectileComponent::Initialize()
{

	if (gEnv->IsGameOrSimulation())
	{
		if (auto * pPhysics = GetEntity()->GetPhysics())
		{
			if (m_autoImpulse)
			{
				const CCamera& systemCamera = gEnv->pSystem->GetViewCamera();

				pe_action_impulse impulse;
				impulse.impulse = systemCamera.GetViewdir() * m_speed;
				impulse.iApplyTime = 0;
				pPhysics->Action(&impulse);
			}
		}
	}
}

EntityEventMask CProjectileComponent::GetEventMask() const
{
	return EEntityEvent::PhysicsCollision | EEntityEvent::Update;
}

void CProjectileComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case EEntityEvent::PhysicsCollision:
	{
		if (EventPhysCollision* pCollision = reinterpret_cast<EventPhysCollision*>(event.nParam[0]))
		{
			if (IEntity* pEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pCollision->pEntity[(int)!event.nParam[1]]))
			{
			}
			SpawnExplosion(pCollision->pt, pCollision->n);
			gEnv->pEntitySystem->RemoveEntity(GetEntityId());
		}
	} break;

	case EEntityEvent::Update:
	{
		if (!gEnv->IsEditing())
		{
			m_lifeTime -= 1 * event.fParam[0];
			if (m_lifeTime <= 0)
			{
				gEnv->pEntitySystem->RemoveEntity(GetEntityId());
			}
		}

	} break;
	}
}

void CProjectileComponent::SpawnExplosion(const Vec3& pt, const Vec3& normal)
{
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(m_explosionClassName.value);
	spawnParams.sName = nullptr;
	spawnParams.vPosition = pt + normal * 0.1f;
	spawnParams.qRotation = Quat::CreateRotationVDir((pt - spawnParams.vPosition).GetNormalized()) * Quat::CreateRotationY(DEG2RAD(cry_random(-360.f, 360.f))) * Quat::CreateRotationX(DEG2RAD(90.f));
	gEnv->pEntitySystem->SpawnEntity(spawnParams);
}
