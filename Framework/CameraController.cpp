//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"
#include "CameraController.h"

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>

#include <CryPhysics/physinterface.h>

#include <CryMath/Random.h>

#include "Framework/PlayerController.h"

#include <ViewSystem/ViewSystem.h>

#include <CryGame/GameUtils.h>

static void RegisterCameraControllerComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CCameraControllerComponent));
		// Functions
		{
		}
	}
}

CCameraControllerComponent* CCameraControllerComponent::m_pInstance = nullptr;

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCameraControllerComponent)

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
}

EntityEventMask CCameraControllerComponent::GetEventMask() const
{
	return EEntityEvent::Update | EEntityEvent::GameplayStarted;
}

IEntityComponent::ComponentEventPriority CCameraControllerComponent::GetEventPriority() const
{
	return ComponentEventPriority();
}

void CCameraControllerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case EEntityEvent::Update:
	{
		if (gEnv->IsGameOrSimulation())
		{
			UpdateInputs();
			UpdateCamera();
		}

	}
	break;

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

void CCameraControllerComponent::UpdateCamera()
{
	if (cameraType == Game::ECameraType::Car)
	{
		// Car update camera
		Vec3 playerPos = CPlayerController::Get()->GetControlledPawn()->GetEntity()->GetWorldPos();
		Matrix34 m_cameraMatrix;

		m_viewPitch += (GetMousePitchDelta() + GetXiPitchDelta()) * 4.f;
		m_viewPitch = clamp_tpl(m_viewPitch, DEG2RAD(m_pitchMin * -1.0f), DEG2RAD(m_pitchMax));
		m_viewYaw += (GetMouseYawDelta() - GetXiYawDelta()) * 4.f;

		if (m_viewYaw > gf_PI)
			m_viewYaw -= gf_PI2;
		if (m_viewYaw < -gf_PI)
			m_viewYaw += gf_PI2;


		Quat quatPreTransYP = Quat(Ang3(m_viewPitch, 0.0f, m_viewYaw));


		Interpolate(m_rightOffset, m_newRightOffset, 1, gEnv->pTimer->GetFrameTime());
		Vec3 rightOffset = GetEntity()->GetForwardDir().cross(Vec3(0.f, 0.f, 1.f)).GetNormalized() * m_rightOffset;

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

		return;
	}

	if (cameraType == Game::ECameraType::ThirdPerson)
	{
		// Third Person update camera
		Vec3 playerPos = CPlayerController::Get()->GetControlledPawn()->GetEntity()->GetWorldPos();
		Matrix34 m_cameraMatrix;

		m_viewPitch += (GetMousePitchDelta() + GetXiPitchDelta()) * 4.f;
		m_viewPitch = clamp_tpl(m_viewPitch, DEG2RAD(m_pitchMin * -1.0f), DEG2RAD(m_pitchMax));
		m_viewYaw += (GetMouseYawDelta() - GetXiYawDelta()) * 4.f;

		if (m_viewYaw > gf_PI)
			m_viewYaw -= gf_PI2;
		if (m_viewYaw < -gf_PI)
			m_viewYaw += gf_PI2;


		Quat quatPreTransYP = Quat(Ang3(m_viewPitch, 0.0f, m_viewYaw));


		Interpolate(m_rightOffset, m_newRightOffset, 6, gEnv->pTimer->GetFrameTime());
		Vec3 rightOffset = GetEntity()->GetForwardDir().cross(Vec3(0.f, 0.f, 1.f)).GetNormalized() * m_rightOffset;
				
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

		return;
	}

	if (m_pCameraRootEntity)
	{
		Vec3 pos = Vec3::CreateLerp(m_pEntity->GetPos(), m_pCameraRootEntity->GetPos(), gEnv->pTimer->GetFrameTime() * followMoveCameraSpeed);
		Quat rot = Quat::CreateSlerp(m_pEntity->GetRotation(), m_pCameraRootEntity->GetRotation(), gEnv->pTimer->GetFrameTime() * followRotateCameraSpeed);

		//Look At
		if (cameraType == Game::ECameraType::LookAt)
		{
			Vec3 playerPos = CPlayerController::Get()->GetControlledPawn()->GetEntity()->GetWorldPos();
			Vec3 vecTargetAimPosition = playerPos + Vec3(0.f, 0.f, 1.8f);
			rot = Quat::CreateRotationVDir((vecTargetAimPosition - pos).GetNormalizedSafe());
		}

		if (cameraType == Game::ECameraType::ViewControl)
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

		if (cameraType == Game::ECameraType::TransformControl)
		{
			// in dev
		}

		Matrix34 camMatrix = Matrix34::Create(Vec3(1.0f), rot, pos);
		m_pEntity->SetLocalTM(camMatrix);
	}

	//WORLD MATRIX TO LOCAL
	//m_pEntity->SetLocalTM(m_pEntity->GetParentAttachPointWorldTM().GetInverted() * camMatrix);
}	

void CCameraControllerComponent::SetOffset(const Vec3& offset)
{
	m_cameraOffsetThirdPerson = offset;
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
		m_saveCameraDistanceThirdPerson = m_newCameraDistanceThirdPerson;
		m_newCameraDistanceThirdPerson = 2.0f;
		m_newRightOffset = 0.6f;
		m_newFieldOfView = m_fieldOfViewExamine;
	}

	if (activationMode == eAAM_OnRelease)
	{
		m_newCameraDistanceThirdPerson = m_saveCameraDistanceThirdPerson;
		m_newRightOffset = 0.0f;
		m_newFieldOfView = m_fieldOfViewDefault;
	}
}

void CCameraControllerComponent::OnTriggerXIRight(int activationMode, float value)
{
	if (value > 0)
	{
		m_saveCameraDistanceThirdPerson = m_newCameraDistanceThirdPerson;
		m_newCameraDistanceThirdPerson = 2.0f;
		m_newRightOffset = 0.6f;
		m_newFieldOfView = m_fieldOfViewExamine;
	}
	else
	{
		m_newCameraDistanceThirdPerson = m_saveCameraDistanceThirdPerson;
		m_newRightOffset = 0.0f;
		m_newFieldOfView = m_fieldOfViewDefault;
	}
}
