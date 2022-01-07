//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"
#include "Car.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

#include <CryPhysics\physinterface.h>
#include <CryGame/GameUtils.h>
#include "Framework/PlayerController.h"
#include "Alex.h"
#include <CryGame/GameUtils.h>

static void RegisterCarPlayer(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CCarPlayer));
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCarPlayer);

CCarPlayer::~CCarPlayer()
{
	ReleaseInputs();
}

void CCarPlayer::Initialize()
{
	m_pVehicleController = GetEntity()->GetOrCreateComponent<CVehicleController>();
}

IEntityComponent::ComponentEventPriority CCarPlayer::GetEventPriority() const
{
	return 16;
}

Cry::Entity::EventFlags CCarPlayer::GetEventMask() const
{
	return EEntityEvent::Update;
}

void CCarPlayer::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case EEntityEvent::Update:
	{	
		//Is visible entity
		//const CCamera& Camera = gEnv->pSystem->GetViewCamera();
		//AABB aabb;
		//GetEntity()->GetWorldBounds(aabb);
		//const uint8 a = Camera.IsAABBVisible_FH(aabb);
		//CryLog(string().Format("%i", a).c_str());

		if (m_pVehicleController && m_bNeutral)
		{
			Vec3 vel = Vec3::CreateLerp(m_pVehicleController->GetVelocity(), ZERO, gEnv->pTimer->GetFrameTime());
			m_pVehicleController->SetVelocity(vel);
		}

		//IPersistantDebug* debug = gEnv->pGameFramework->GetIPersistantDebug();
		//debug->Begin("player_car", true);
		//debug->Add2DText(string().Format("engine rpm: %f", GetEngineRPM()).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());
		//debug->Add2DText(string().Format("engine gear: %i", GetCurrentGear()).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());
		//debug->Add2DText(string().Format("engine throttle: %f", GetThrottle()).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());
		//debug->Add2DText(string().Format("brake: %f", GetFootBrake()).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());
		//debug->Add2DText(string().Format("vel: %f", m_pVehiclePhysics->GetVelocity().len2()).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());
	}
	break;
	}
}

int CCarPlayer::GetCurrentGear()
{
	pe_status_vehicle m_vehicleStatus;
	m_pEntity->GetPhysicalEntity()->GetStatus(&m_vehicleStatus);
	return m_vehicleStatus.iCurGear;
}

float CCarPlayer::GetEngineRPM()
{
	pe_status_vehicle m_vehicleStatus;
	m_pEntity->GetPhysicalEntity()->GetStatus(&m_vehicleStatus);
	return m_vehicleStatus.engineRPM;
}

float CCarPlayer::GetThrottle()
{
	pe_status_vehicle m_vehicleStatus;
	m_pEntity->GetPhysicalEntity()->GetStatus(&m_vehicleStatus);
	return m_vehicleStatus.pedal;
}

float CCarPlayer::GetFootBrake()
{
	pe_status_vehicle m_vehicleStatus;
	m_pEntity->GetPhysicalEntity()->GetStatus(&m_vehicleStatus);
	return m_vehicleStatus.footbrake;
}

void CCarPlayer::SetDriver(IEntity* driverEntity)
{
	GetEntity()->AttachChild(driverEntity);
	driverEntity->SetRotation(Quat::CreateRotationVDir(GetEntity()->GetRightDir()));
	m_pCarDriverEntity = driverEntity;
}

void CCarPlayer::ReleaseDriver()
{
	GetEntity()->DetachAll();
	CPlayerController::Get()->SetControlledPawn(m_pCarDriverEntity->GetComponent<CAlexPlayer>());
	m_pCarDriverEntity->GetComponent<CAlexPlayer>()->GetOutTransport();
	GameEvents::CPawnEvents::Get()->SendEvent(GameEvents::EPawnEvent::Pawn_ReleaseDriving);
	m_pCarDriverEntity = nullptr;
}

void CCarPlayer::OnForward(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (m_pVehicleController)
		{
			m_pVehicleController->SetThrottle(1.0f);
			m_bNeutral = false;
		}
	}
	if (activationMode == eAAM_OnRelease)
	{
		if (m_pVehicleController)
		{
			m_pVehicleController->SetThrottle(0.0f);
			m_bNeutral = true;
		}
	}
}

void CCarPlayer::OnBackward(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (m_pVehicleController)
		{
			m_pVehicleController->SetCurrentGear(1);
			m_pVehicleController->SetBrake(1.0f);
			m_bNeutral = false;
		}
	}
	if (activationMode == eAAM_OnRelease)
	{
		if (m_pVehicleController)
		{
			m_pVehicleController->SetThrottle(0.0f);
			m_bNeutral = true;
		}
	}
}

void CCarPlayer::OnLeft(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (m_pVehicleController)
		{
			float angle = -value * 40.0f;
			m_pVehicleController->SetSteeringAngle(angle);
		}
	}
	if (activationMode == eAAM_OnRelease)
	{
		if (m_pVehicleController)
		{
			m_pVehicleController->SetSteeringAngle(0);
		}
	}
}

void CCarPlayer::OnRight(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (m_pVehicleController)
		{
			float angle = value * 40.0f;
			m_pVehicleController->SetSteeringAngle(angle);
		}
	}
	if (activationMode == eAAM_OnRelease)
	{
		if (m_pVehicleController)
		{
			m_pVehicleController->SetSteeringAngle(0);
		}
	}
}

void CCarPlayer::OnYawDeltaXILeft(int activationMode, float value)
{
	if (m_pVehicleController)
	{
		float angle = value * 40.0f;
		m_pVehicleController->SetSteeringAngle(angle);
	}
}

void CCarPlayer::OnPitchDeltaXILeft(int activationMode, float value)
{
}

void CCarPlayer::OnMouseX(int activationMode, float value)
{
}

void CCarPlayer::OnMouseY(int activationMode, float value)
{
}

void CCarPlayer::OnYawDeltaXIRight(int activationMode, float value)
{
}

void CCarPlayer::OnPitchDeltaXIRight(int activationMode, float value)
{
}

void CCarPlayer::OnEnter(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		ReleaseDriver();
	}
}

void CCarPlayer::OnTriggerXIRight(int activationMode, float value)
{
	if (m_pVehicleController)
	{
		if (value > 0.0f)
		{
			m_pVehicleController->SetThrottle(value);
			m_bNeutral = false;
		}
		else
		{
			m_pVehicleController->SetThrottle(0.0f);
			m_bNeutral = true;
		}
	}
}