//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"
#include "CameraController.h"

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>

#include <CryPhysics/physinterface.h>

#include <CryMath/Random.h>

#include "Framework/PlayerController.h"

#include <ViewSystem/ViewSystem.h>

#include <CryGame/GameUtils.h>


//REGISTER SIGNALS
void ReflectType(Schematyc::CTypeDesc<CCameraControllerComponent::SPressSecondaryInteractSignal>& desc)
{
	desc.SetGUID("{E0F48F5D-F679-4B08-B585-A87AFB541105}"_cry_guid);
	desc.SetLabel("OnPressSecondaryInteract");
}

void ReflectType(Schematyc::CTypeDesc<CCameraControllerComponent::SReleaseSecondaryInteractSignal>& desc)
{
	desc.SetGUID("{1056D2B1-C714-469B-80DE-8A4464A6FA88}"_cry_guid);
	desc.SetLabel("OnReleaseSecondaryInteract");
}

void ReflectType(Schematyc::CTypeDesc<CCameraControllerComponent::SPawnStartFightSignal>& desc)
{
	desc.SetGUID("{3FCD0E13-47FC-4D73-93C5-E9DB62F1EE9F}"_cry_guid);
	desc.SetLabel("OnPawnStartFight");
}

void ReflectType(Schematyc::CTypeDesc<CCameraControllerComponent::SPawnReleaseFightSignal>& desc)
{
	desc.SetGUID("{E03821C7-A409-4FAB-BF16-73EDFB8A279E}"_cry_guid);
	desc.SetLabel("OnPawnReleaseFight");
}

void ReflectType(Schematyc::CTypeDesc<CCameraControllerComponent::SPawnSetDrivingSignal>& desc)
{
	desc.SetGUID("{414665AC-B65F-48E3-8E70-21BF8E55535D}"_cry_guid);
	desc.SetLabel("OnPawnSetDriving");
}

void ReflectType(Schematyc::CTypeDesc<CCameraControllerComponent::SPawnReleaseDrivingSignal>& desc)
{
	desc.SetGUID("{8A702DA1-F8BF-4F1C-9CF8-E8E6C7C7302B}"_cry_guid);
	desc.SetLabel("OnPawnReleaseDriving");
}
//~REGISTER SIGNALS

static void RegisterCameraControllerComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CCameraControllerComponent));
		// Functions
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CCameraControllerComponent::UpdateCinematic, "{C636E38B-7594-46C2-8BE5-DA9DCCE80BD0}"_cry_guid, "UpdateCinematic");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CCameraControllerComponent::UpdateThirdPerson, "{57A3D9FE-0151-4AC5-BE44-8140294669B2}"_cry_guid, "UpdateThirdPerson");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CCameraControllerComponent::UpdateVehicle, "{73CFF915-8B0C-4F02-A93A-B7E98B29EFB8}"_cry_guid, "UpdateVehicle");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CCameraControllerComponent::UpdateInputs, "{A8CDFFAD-9B34-4C92-BBFA-9C34834AE3A3}"_cry_guid, "UpdateInputs");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CCameraControllerComponent::SetFoV, "{B591E6C3-FC6D-49DA-A063-5EF607CDEB44}"_cry_guid, "SetFoV");
			pFunction->SetFlags({ Schematyc::EEnvFunctionFlags::Member });
			pFunction->BindInput(1, 'fov', "FoV");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CCameraControllerComponent::SetRightOffsetTP, "{241DFCB0-9C36-4E91-825D-07015A667AD4}"_cry_guid, "SetRightOffsetTP");
			pFunction->SetFlags({ Schematyc::EEnvFunctionFlags::Member });
			pFunction->BindInput(1, 'ofst', "Offset");
			componentScope.Register(pFunction);
		}
		{
			auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CCameraControllerComponent::SetCameraDistanceTP, "{BFF3F472-78FD-46E6-92ED-3615134D012D}"_cry_guid, "SetCameraDistanceTP");
			pFunction->SetFlags({ Schematyc::EEnvFunctionFlags::Member });
			pFunction->BindInput(1, 'dst', "Distance");
			componentScope.Register(pFunction);
		}
		//Signals
		{
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(CCameraControllerComponent::SPressSecondaryInteractSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(CCameraControllerComponent::SReleaseSecondaryInteractSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(CCameraControllerComponent::SPawnStartFightSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(CCameraControllerComponent::SPawnReleaseFightSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(CCameraControllerComponent::SPawnSetDrivingSignal));
			componentScope.Register(SCHEMATYC_MAKE_ENV_SIGNAL(CCameraControllerComponent::SPawnReleaseDrivingSignal));
		}
	}
}

CCameraControllerComponent* CCameraControllerComponent::m_pInstance = nullptr;

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCameraControllerComponent)

CCameraControllerComponent::CCameraControllerComponent()
{

}

CCameraControllerComponent::~CCameraControllerComponent()
{

}

void CCameraControllerComponent::Initialize()
{
	if (m_pInstance)
	{
		if (m_pInstance->m_pEntity == m_pEntity)
			return;

		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "More than one camera controller is not allowed. Camera controller removed from entity '%s'.", m_pInstance->m_pEntity->GetName());
		m_pInstance->m_pEntity->RemoveComponent<CCameraControllerComponent>();
	}
	m_pInstance = this;

	m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
	m_pCameraComponent->SetFarPlane(m_cameraFarPlane);
	m_pCameraComponent->SetFieldOfView(CryTransform::CAngle::FromDegrees(m_fieldOfViewDefault));
}

EntityEventMask CCameraControllerComponent::GetEventMask() const
{
	return EEntityEvent::GameplayStarted;
}

IEntityComponent::ComponentEventPriority CCameraControllerComponent::GetEventPriority() const
{
	return ComponentEventPriority();
}

void CCameraControllerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case EEntityEvent::GameplayStarted:
	{
		if (gEnv->pEntitySystem->FindEntityByName(m_cameraRootName.c_str()))
		{
			m_pCameraRootEntity = gEnv->pEntitySystem->FindEntityByName(m_cameraRootName.c_str());
		}
		if (CPlayerController::Get()->GetControlledPawn()->GetEntity())
		{
			m_quatTargetRotation = Quat::CreateRotationVDir(CPlayerController::Get()->GetControlledPawn()->GetEntity()->GetForwardDir());
		}

		m_quatLastTargetRotation = m_quatTargetRotation;

		m_newCameraDistanceThirdPerson = m_cameraDistanceThirdPerson;

		m_newFieldOfView = m_fieldOfViewDefault;
	}
	break;
	}
}

void CCameraControllerComponent::UpdateVehicle()
{
	Vec3 playerPos = CPlayerController::Get()->GetControlledPawn()->GetEntity()->GetWorldPos();
	Matrix34 m_cameraMatrix;

	m_viewPitch += (GetMousePitchDelta() + GetXiPitchDelta()) * 4.f;
	m_viewPitch = clamp_tpl(m_viewPitch, DEG2RAD(m_pitchMinVehicle * -1.0f), DEG2RAD(m_pitchMaxVehicle));
	m_viewYaw += (GetMouseYawDelta() - GetXiYawDelta()) * 4.f;

	if (m_viewYaw > gf_PI)
		m_viewYaw -= gf_PI2;
	if (m_viewYaw < -gf_PI)
		m_viewYaw += gf_PI2;


	Quat quatPreTransYP = Quat(Ang3(m_viewPitch, 0.0f, m_viewYaw));


	Interpolate(m_rightOffsetVehicle, m_newRightOffsetVehicle, 1, gEnv->pTimer->GetFrameTime());
	Vec3 rightOffset = GetEntity()->GetForwardDir().cross(Vec3(0.f, 0.f, 1.f)).GetNormalized() * m_rightOffsetVehicle;

	Vec3 vecTargetAimPosition = playerPos + Vec3(0.f, 0.f, 1.7f);

	Quat quatTargetRotationGoal = m_quatTargetRotation * quatPreTransYP;
	Quat quatTargetRotation = Quat::CreateSlerp(m_quatLastTargetRotation, quatTargetRotationGoal, gEnv->pTimer->GetFrameTime() * 7.0f);
	m_quatLastTargetRotation = quatTargetRotation;

	Vec3 vecViewPosition = vecTargetAimPosition + (quatTargetRotation * (FORWARD_DIRECTION * 7.0f));

	Quat quatViewRotationGoal = Quat::CreateRotationVDir((vecTargetAimPosition - vecViewPosition).GetNormalizedSafe());
	Quat quatViewRotation = Quat::CreateSlerp(m_quatLastViewRotation, quatViewRotationGoal, gEnv->pTimer->GetFrameTime() * 50.f);
	m_quatLastViewRotation = quatViewRotation;

	Quat quatOrbitRotation = quatViewRotation;

	CollisionDetection(vecTargetAimPosition, vecViewPosition);

	m_vecLastPosition = vecViewPosition;
	m_cameraMatrix = Matrix34::Create(Vec3(1.0f), quatOrbitRotation, vecViewPosition);

	m_pEntity->SetLocalTM(m_cameraMatrix);
}

void CCameraControllerComponent::UpdateThirdPerson()
{
	if (IEntity* playerEntity = CPlayerController::Get()->GetControlledPawn()->GetEntity())
	{
		Vec3 playerPos = playerEntity->GetWorldPos();
		Matrix34 m_cameraMatrix;

		m_viewPitch += (GetMousePitchDelta() + GetXiPitchDelta()) * 4.f;
		m_viewPitch = clamp_tpl(m_viewPitch, DEG2RAD(m_pitchMinThirdPerson * -1.0f), DEG2RAD(m_pitchMaxThirdPerson));
		m_viewYaw += (GetMouseYawDelta() - GetXiYawDelta()) * 4.f;

		if (m_viewYaw > gf_PI)
			m_viewYaw -= gf_PI2;
		if (m_viewYaw < -gf_PI)
			m_viewYaw += gf_PI2;


		Quat quatPreTransYP = Quat(Ang3(m_viewPitch, 0.0f, m_viewYaw));


		Interpolate(m_rightOffsetThirdPerson, m_newRightOffsetThirdPerson, 6, gEnv->pTimer->GetFrameTime());
		Vec3 rightOffset = GetEntity()->GetForwardDir().cross(Vec3(0.f, 0.f, 1.f)).GetNormalized() * m_rightOffsetThirdPerson;

		Vec3 vecTargetAimPosition = playerPos + Vec3(0.f, 0.f, 1.55f) + rightOffset;

		Quat quatTargetRotationGoal = m_quatTargetRotation * quatPreTransYP;
		Quat quatTargetRotation = Quat::CreateSlerp(m_quatLastTargetRotation, quatTargetRotationGoal, gEnv->pTimer->GetFrameTime() * 7.0f);
		m_quatLastTargetRotation = quatTargetRotation;

		Interpolate(m_cameraDistanceThirdPerson, m_newCameraDistanceThirdPerson, 6, gEnv->pTimer->GetFrameTime());

		Vec3 vecViewPosition = vecTargetAimPosition + (quatTargetRotation * (FORWARD_DIRECTION * m_cameraDistanceThirdPerson));

		Quat quatViewRotationGoal = Quat::CreateRotationVDir((vecTargetAimPosition - vecViewPosition).GetNormalizedSafe());
		Quat quatViewRotation = Quat::CreateSlerp(m_quatLastViewRotation, quatViewRotationGoal, gEnv->pTimer->GetFrameTime() * 50.f);
		m_quatLastViewRotation = quatViewRotation;

		Quat quatOrbitRotation = quatViewRotation;

		CollisionDetection(vecTargetAimPosition, vecViewPosition);

		m_vecLastPosition = vecViewPosition;
		m_cameraMatrix = Matrix34::Create(Vec3(1.0f), quatOrbitRotation, vecViewPosition);

		m_pEntity->SetLocalTM(m_cameraMatrix);

		//FOV
		Interpolate(m_fieldOfView, m_newFieldOfView, 6, gEnv->pTimer->GetFrameTime());
		m_pCameraComponent->SetFieldOfView(CryTransform::CAngle::FromDegrees(m_fieldOfView));
	}	
}

void CCameraControllerComponent::UpdateCinematic()
{
	if (m_pCameraRootEntity)
	{
		Vec3 pos = Vec3::CreateLerp(m_pEntity->GetPos(), m_pCameraRootEntity->GetPos(), gEnv->pTimer->GetFrameTime() * followMoveCameraSpeed);
		Quat rot = Quat::CreateSlerp(m_pEntity->GetRotation(), m_pCameraRootEntity->GetRotation(), gEnv->pTimer->GetFrameTime() * followRotateCameraSpeed);

		//Look At
		if (m_isLookAt)
		{
			Vec3 playerPos = CPlayerController::Get()->GetControlledPawn()->GetEntity()->GetWorldPos();
			Vec3 vecTargetAimPosition = playerPos + Vec3(0.f, 0.f, 1.8f);
			rot = Quat::CreateRotationVDir((vecTargetAimPosition - pos).GetNormalizedSafe());
		}

		if (m_isViewControl)
		{
			m_viewPitch += (GetMousePitchDelta() + GetXiPitchDelta()) * 4.0f;
			//m_viewPitch = clamp_tpl(m_viewPitch, DEG2RAD(5.0f), DEG2RAD(60.0f));
			m_viewYaw += (GetMouseYawDelta() - GetXiYawDelta()) * 4.0f;
			//m_viewYaw = clamp_tpl(m_viewYaw, DEG2RAD(-90.0f), DEG2RAD(90.0f));

			if (m_viewYaw > gf_PI)
				m_viewYaw -= gf_PI2;
			if (m_viewYaw < -gf_PI)
				m_viewYaw += gf_PI2;

			Quat quatPreTransYP = Quat(Ang3(m_viewPitch * -1.0f, 0.0f, m_viewYaw));

			Quat quatTargetRotationGoal = m_quatTargetRotation * quatPreTransYP;
			Quat quatTargetRotation;

			quatTargetRotation = Quat::CreateSlerp(m_quatLastTargetRotation, quatTargetRotationGoal, gEnv->pTimer->GetFrameTime() * 5.0f);
			m_quatLastTargetRotation = quatTargetRotation;

			rot = quatTargetRotation;
		}

		Matrix34 camMatrix = Matrix34::Create(Vec3(1.0f), rot, pos);
		m_pEntity->SetLocalTM(camMatrix);

		//WORLD MATRIX TO LOCAL
		//m_pEntity->SetLocalTM(m_pEntity->GetParentAttachPointWorldTM().GetInverted() * camMatrix);
	}
}

void CCameraControllerComponent::CameraZoom(const float& value)
{
	m_cameraDistanceThirdPerson -= crymath::clamp(value, -1.f, 1.f);
	m_cameraDistanceThirdPerson = crymath::clamp(m_cameraDistanceThirdPerson, 2.f, 20.f);
}

bool CCameraControllerComponent::CollisionDetection(const Vec3& TargetPosition, Vec3& CameraPosition)
{
	auto pPlayer = CPlayerController::Get()->GetControlledPawn()->GetEntity();
	ray_hit rayhit;
	static IPhysicalEntity* pSkipEnts[10];
	pSkipEnts[0] = pPlayer->GetPhysics();

	int hits = gEnv->pPhysicalWorld->RayWorldIntersection(TargetPosition,
		CameraPosition - TargetPosition,
		ent_static | ent_sleeping_rigid | ent_rigid | ent_independent | ent_terrain,
		rwi_stop_at_pierceable | rwi_colltype_any,
		&rayhit,
		1, pSkipEnts, 2);

	if (hits)
	{
		CameraPosition = rayhit.pt;
		return true;
	}
	return false;
}


// Input methods
void CCameraControllerComponent::UpdateInputs()
{
	static float frameTime = gEnv->pTimer->GetFrameTime();

	// We can just add up all the acculmated requests to find out how much pitch / yaw is being requested.
	// It's also a good time to filter out any small movement requests to stabilise the camera / etc.
	m_lastPitchDelta = m_mousePitchDelta + m_xiPitchDelta;
	if (std::abs(m_lastPitchDelta) < m_pitchFilter)
		m_lastPitchDelta = 0.0f;
	m_lastYawDelta = m_mouseYawDelta + m_xiYawDelta;
	if (std::abs(m_lastYawDelta) < m_yawFilter)
		m_lastYawDelta = 0.0f;

	// Track the last values for mouse and xbox inputs. They're filtered individually for low level noise.
	m_lastMousePitchDelta = std::abs(m_mousePitchDelta) >= m_pitchFilter ? m_mousePitchDelta : 0.0f;
	m_lastMouseYawDelta = std::abs(m_mouseYawDelta) >= m_yawFilter ? m_mouseYawDelta : 0.0f;
	m_lastXiPitchDelta = std::abs(m_xiPitchDelta) >= m_pitchFilter ? m_xiPitchDelta : 0.0f;
	m_lastXiYawDelta = std::abs(m_xiYawDelta) >= m_yawFilter ? m_xiYawDelta : 0.0f;

	// Circle of life!
	m_mousePitchDelta = m_mouseYawDelta = 0.0f;
}

void CCameraControllerComponent::SetFoV(float fov)
{
	m_newFieldOfView = fov;
}

void CCameraControllerComponent::SetRightOffsetTP(float val)
{
	m_newRightOffsetThirdPerson = val;
}

void CCameraControllerComponent::SetCameraDistanceTP(float val)
{
	m_newCameraDistanceThirdPerson = val;
}


Ang3 CCameraControllerComponent::GetRotationDelta()
{
	return Ang3(m_lastPitchDelta, 0.0f, m_lastYawDelta);
}

float CCameraControllerComponent::GetPitchDelta()
{
	return m_lastPitchDelta;
}

float CCameraControllerComponent::GetYawDelta()
{
	return m_lastYawDelta;
}

float CCameraControllerComponent::GetMousePitchDelta()
{
	return m_lastMousePitchDelta;
}

float CCameraControllerComponent::GetMouseYawDelta()
{
	return m_lastMouseYawDelta;
}

float CCameraControllerComponent::GetXiPitchDelta()
{
	return m_lastXiPitchDelta;
}

float CCameraControllerComponent::GetXiYawDelta()
{
	return m_lastXiYawDelta;
}



void CCameraControllerComponent::OnMouseX(int activationMode, float value)
{
	float cl_mouseSensitivity = 3.0f;

	// Sensible scaling value for rotations in radians.
	float mouseSensitivity = 0.00032f * max(0.01f, cl_mouseSensitivity * m_mousePitchYawSensitivity);

	// Add the yaw delta.
	m_mouseYawDelta -= value * mouseSensitivity;
}

void CCameraControllerComponent::OnMouseY(int activationMode, float value)
{
	float cl_mouseSensitivity = 3.0f;

	// Sensible scaling value for rotations in radians.
	float mouseSensitivity = 0.00032f * max(0.01f, cl_mouseSensitivity * m_mousePitchYawSensitivity);

	// Add the delta, taking into account mouse inversion.  Clamp the result.
	float invertYAxis = m_mouseInvertPitch ? -1.0f : 1.0f;
	m_mousePitchDelta += value * mouseSensitivity * invertYAxis;
}

void CCameraControllerComponent::OnYawDeltaXIRight(int activationMode, float value)
{
	float radians = DEG2RAD(value);

	if (std::abs(radians) < m_xiYawFilter)
		m_xiYawDelta = 0.0f;
	else
		m_xiYawDelta = radians;
}

void CCameraControllerComponent::OnPitchDeltaXIRight(int activationMode, float value)
{
	float radians = DEG2RAD(value * -1.0f);

	if (std::abs(radians) < m_xiPitchFilter)
		m_xiPitchDelta = 0.0f;
	else
		m_xiPitchDelta = radians;
}

void CCameraControllerComponent::OnMouseWheel(int activationMode, float value)
{
	m_newCameraDistanceThirdPerson -= crymath::clamp(value, -1.f, 1.f) * 0.15f;
	m_newCameraDistanceThirdPerson = crymath::clamp(m_newCameraDistanceThirdPerson, 1.f, 20.f);
}

void CCameraControllerComponent::OnMouseButtonRight(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (Schematyc::IObject* pSchematycObject = GetEntity()->GetSchematycObject())
		{
			pSchematycObject->ProcessSignal(SPressSecondaryInteractSignal(), GetGUID());
		}
	}
	if (activationMode == eAAM_OnRelease)
	{
		if (Schematyc::IObject* pSchematycObject = GetEntity()->GetSchematycObject())
		{
			pSchematycObject->ProcessSignal(SReleaseSecondaryInteractSignal(), GetGUID());
		}
	}
}

void CCameraControllerComponent::OnTriggerXIRight(int activationMode, float value)
{
	if (value > 0)
	{
		if (Schematyc::IObject* pSchematycObject = GetEntity()->GetSchematycObject())
		{
			pSchematycObject->ProcessSignal(SPressSecondaryInteractSignal(), GetGUID());
		}
	}
	else
	{
		if (Schematyc::IObject* pSchematycObject = GetEntity()->GetSchematycObject())
		{
			pSchematycObject->ProcessSignal(SReleaseSecondaryInteractSignal(), GetGUID());
		}
	}
}

void CCameraControllerComponent::OnPawnStartFight()
{
	if (Schematyc::IObject* pSchematycObject = GetEntity()->GetSchematycObject())
	{
		pSchematycObject->ProcessSignal(SPawnStartFightSignal(), GetGUID());
	}
}

void CCameraControllerComponent::OnPawnReleaseFight()
{
	if (Schematyc::IObject* pSchematycObject = GetEntity()->GetSchematycObject())
	{
		pSchematycObject->ProcessSignal(SPawnReleaseFightSignal(), GetGUID());
	}
}

void CCameraControllerComponent::OnPawnSetDriving()
{
	if (Schematyc::IObject* pSchematycObject = GetEntity()->GetSchematycObject())
	{
		pSchematycObject->ProcessSignal(SPawnSetDrivingSignal(), GetGUID());
	}
}

void CCameraControllerComponent::OnPawnReleaseDriving()
{
	if (Schematyc::IObject* pSchematycObject = GetEntity()->GetSchematycObject())
	{
		pSchematycObject->ProcessSignal(SPawnReleaseDrivingSignal(), GetGUID());
	}
}
