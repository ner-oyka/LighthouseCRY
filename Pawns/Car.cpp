//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"
#include "Car.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

#include <CryPhysics\physinterface.h>

#include "Alex.h"

static void RegisterCarPlayer(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CCarPlayer));
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCarPlayer);

void CCarPlayer::Initialize()
{
	m_pVehiclePhysics = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CVehiclePhysicsComponent>();
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
		//IPersistantDebug* debug = gEnv->pGameFramework->GetIPersistantDebug();
		//debug->Begin("player_car", true);
		//debug->Add2DText(string().Format("engine rpm: %f", GetEngineRPM()).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());
		//debug->Add2DText(string().Format("engine gear: %i", GetCurrentGear()).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());
		//debug->Add2DText(string().Format("engine throttle: %f", GetThrottle()).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());
		//debug->Add2DText(string().Format("brake: %f", GetFootBrake()).c_str(), 1.5f, ColorF(0.2f, 0.7f, 0.2f), gEnv->pTimer->GetFrameTime());
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
	m_pCarDriverEntity = nullptr;
}

void CCarPlayer::OnForward(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (m_pVehiclePhysics)
		{
			m_pVehiclePhysics->UseHandbrake(false);
			m_pVehiclePhysics->SetBrake(0.f);
			m_pVehiclePhysics->SetThrottle(1.0f);
		}
	}
	if (activationMode == eAAM_OnRelease)
	{
		if (m_pVehiclePhysics)
		{
			m_pVehiclePhysics->SetThrottle(0.0f);
			//m_pVehiclePhysics->UseHandbrake(true);
			//pe_action_drive driveAction;
			//driveAction.clutch = 1.f;
			//m_pEntity->GetPhysicalEntity()->Action(&driveAction);

			m_pVehiclePhysics->GearDown();
			m_pVehiclePhysics->SetBrake(1.f);

		}
	}
}

void CCarPlayer::OnBackward(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (m_pVehiclePhysics)
		{
			m_pVehiclePhysics->UseHandbrake(false);
			m_pVehiclePhysics->SetThrottle(-0.2f);
		}
	}
	if (activationMode == eAAM_OnRelease)
	{
		if (m_pVehiclePhysics)
		{
			m_pVehiclePhysics->UseHandbrake(true);
			m_pVehiclePhysics->SetThrottle(0.0f);
		}
	}
}

void CCarPlayer::OnLeft(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (m_pVehiclePhysics)
		{
			CryTransform::CAngle angle = CryTransform::CAngle::FromDegrees(-value * 30.0f);
			m_pVehiclePhysics->SetSteeringAngle(angle);
		}
	}
	if (activationMode == eAAM_OnRelease)
	{
		if (m_pVehiclePhysics)
		{
			CryTransform::CAngle angle = CryTransform::CAngle::FromDegrees(0);
			m_pVehiclePhysics->SetSteeringAngle(angle);
		}
	}
}

void CCarPlayer::OnRight(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (m_pVehiclePhysics)
		{
			CryTransform::CAngle angle = CryTransform::CAngle::FromDegrees(value * 30.0f);
			m_pVehiclePhysics->SetSteeringAngle(angle);
		}
	}
	if (activationMode == eAAM_OnRelease)
	{
		if (m_pVehiclePhysics)
		{
			CryTransform::CAngle angle = CryTransform::CAngle::FromDegrees(0);
			m_pVehiclePhysics->SetSteeringAngle(angle);
		}
	}
}

void CCarPlayer::OnYawDeltaXILeft(int activationMode, float value)
{
	if (m_pVehiclePhysics)
	{
		CryTransform::CAngle angle = CryTransform::CAngle::FromDegrees(value * 30.0f);
		m_pVehiclePhysics->SetSteeringAngle(angle);
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
	if (m_pVehiclePhysics)
	{
		if (value > 0.0f)
		{
			m_pVehiclePhysics->UseHandbrake(false);
			m_pVehiclePhysics->SetThrottle(value);
		}
		else
		{
			m_pVehiclePhysics->UseHandbrake(true);
			m_pVehiclePhysics->SetBrake(0.5f);
			m_pVehiclePhysics->SetThrottle(0.0f);
		}
	}
}
