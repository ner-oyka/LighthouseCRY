//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"
#include "Car.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>

#include <CryPhysics\physinterface.h>
#include <CryGame/GameUtils.h>

#include "Alex.h"

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
	m_pVehiclePhysics = m_pEntity->GetComponent<Cry::DefaultComponents::CVehiclePhysicsComponent>();
	m_pAnimationComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	m_pEntity->GetAllComponents<Cry::DefaultComponents::CWheelComponent>(m_pWheelsComponents);

	InitializeWheels();

	CryCreateClassInstanceForInterface(cryiidof<IAnimationOperatorQueue>(), m_poseWheelsModifier);
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

		UpdateWheels();

		if (m_bNeutral)
		{
			Vec3 vel = Vec3::CreateLerp(m_pVehiclePhysics->GetVelocity(), ZERO, gEnv->pTimer->GetFrameTime());
			m_pVehiclePhysics->SetVelocity(vel);
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
	CCameraControllerComponent::Get()->SetType(Game::ECameraType::ThirdPerson);
	m_pCarDriverEntity = nullptr;
}

void CCarPlayer::InitializeWheels()
{
	for (Cry::DefaultComponents::CWheelComponent* wheel : m_pWheelsComponents)
	{
		if (ICharacterInstance* pCharacter = m_pAnimationComponent->GetCharacter())
		{
			if (wheel->m_axleIndex == 0)
			{
				if (IAttachment* pAttachment = pCharacter->GetIAttachmentManager()->GetInterfaceByName("Wheel_ForwardL"))
				{
					const QuatT rel = pAttachment->GetAttAbsoluteDefault();
					Matrix34 mat = Matrix34(rel);
					wheel->SetTransformMatrix(mat);
				}
			}
			if (wheel->m_axleIndex == 1)
			{
				if (IAttachment* pAttachment = pCharacter->GetIAttachmentManager()->GetInterfaceByName("Wheel_ForwardR"))
				{
					const QuatT rel = pAttachment->GetAttAbsoluteDefault();
					Matrix34 mat = Matrix34(rel);
					wheel->SetTransformMatrix(mat);
				}
			}
			if (wheel->m_axleIndex == 2)
			{
				if (IAttachment* pAttachment = pCharacter->GetIAttachmentManager()->GetInterfaceByName("Wheel_BackL"))
				{
					const QuatT rel = pAttachment->GetAttAbsoluteDefault();
					Matrix34 mat = Matrix34(rel);
					wheel->SetTransformMatrix(mat);
				}
			}
			if (wheel->m_axleIndex == 3)
			{
				if (IAttachment* pAttachment = pCharacter->GetIAttachmentManager()->GetInterfaceByName("Wheel_BackR"))
				{
					const QuatT rel = pAttachment->GetAttAbsoluteDefault();
					Matrix34 mat = Matrix34(rel);
					wheel->SetTransformMatrix(mat);
				}
			}
		}

	}
}

void CCarPlayer::UpdateWheels()
{
	if (ICharacterInstance* pCharacterInstance = m_pAnimationComponent->GetCharacter())
	{
		pCharacterInstance->GetISkeletonAnim()->PushPoseModifier(2, m_poseWheelsModifier, "Pose Wheels");

		ISkeletonPose* pSkeleton = pCharacterInstance->GetISkeletonPose();
		IDefaultSkeleton& rIDefaultSkeleton = pCharacterInstance->GetIDefaultSkeleton();

		int m_wheelForwardLeftBoneId = rIDefaultSkeleton.GetJointIDByName("Wheel_ForwardLeft");
		int m_wheelForwardRightBoneId = rIDefaultSkeleton.GetJointIDByName("Wheel_ForwardRight");
		int m_wheelBackLeftBoneId = rIDefaultSkeleton.GetJointIDByName("Wheel_BackLeft");
		int m_wheelBackRightBoneId = rIDefaultSkeleton.GetJointIDByName("Wheel_BackRight");

		QuatT relForwardLeft = pSkeleton->GetAbsJointByID(m_wheelForwardLeftBoneId);
		QuatT relForwardRight = pSkeleton->GetAbsJointByID(m_wheelForwardRightBoneId);
		QuatT relBackLeft = pSkeleton->GetAbsJointByID(m_wheelBackLeftBoneId);
		QuatT relBackRight = pSkeleton->GetAbsJointByID(m_wheelBackRightBoneId);

		for (Cry::DefaultComponents::CWheelComponent* wheel : m_pWheelsComponents)
		{
			pe_status_wheel sw;
			pe_status_pos sp;
			sw.partid = m_pEntity->GetPhysicalEntityPartId0(wheel->GetEntitySlotId());
			sp.partid = m_pEntity->GetPhysicalEntityPartId0(wheel->GetEntitySlotId());

			m_pEntity->GetPhysicalEntity()->GetStatus(&sw);
			m_pEntity->GetPhysicalEntity()->GetStatus(&sp);

			if (wheel->m_axleIndex == 0)
			{
				relForwardLeft.q = sp.q;
				relForwardLeft.t = sp.pos;
			}
			if (wheel->m_axleIndex == 1)
			{
				relForwardRight.q = sp.q;
				relForwardRight.t = sp.pos;
			}
			if (wheel->m_axleIndex == 2)
			{
				relBackLeft.q = sp.q;
				relBackLeft.t = sp.pos;
			}
			if (wheel->m_axleIndex == 3)
			{
				relBackRight.q = sp.q;
				relBackRight.t = sp.pos;
			}
		}

		m_poseWheelsModifier->PushOrientation(m_wheelForwardLeftBoneId, IAnimationOperatorQueue::eOp_OverrideWorld, relForwardLeft.q);
		m_poseWheelsModifier->PushOrientation(m_wheelForwardRightBoneId, IAnimationOperatorQueue::eOp_OverrideWorld, relForwardRight.q);
		m_poseWheelsModifier->PushOrientation(m_wheelBackLeftBoneId, IAnimationOperatorQueue::eOp_OverrideWorld, relBackLeft.q);
		m_poseWheelsModifier->PushOrientation(m_wheelBackRightBoneId, IAnimationOperatorQueue::eOp_OverrideWorld, relBackRight.q);

		m_poseWheelsModifier->PushPosition(m_wheelForwardLeftBoneId, IAnimationOperatorQueue::eOp_OverrideWorld, relForwardLeft.t);
		m_poseWheelsModifier->PushPosition(m_wheelForwardRightBoneId, IAnimationOperatorQueue::eOp_OverrideWorld, relForwardRight.t);
		m_poseWheelsModifier->PushPosition(m_wheelBackLeftBoneId, IAnimationOperatorQueue::eOp_OverrideWorld, relBackLeft.t);
		m_poseWheelsModifier->PushPosition(m_wheelBackRightBoneId, IAnimationOperatorQueue::eOp_OverrideWorld, relBackRight.t);
	}
}

void CCarPlayer::OnForward(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (m_pVehiclePhysics)
		{
			m_pVehiclePhysics->SetThrottle(1.0f);
			m_bNeutral = false;
		}
	}
	if (activationMode == eAAM_OnRelease)
	{
		if (m_pVehiclePhysics)
		{
			m_pVehiclePhysics->SetThrottle(0.0f);
			m_bNeutral = true;
		}
	}
}

void CCarPlayer::OnBackward(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		if (m_pVehiclePhysics)
		{
			m_pVehiclePhysics->SetCurrentGear(1);
			m_pVehiclePhysics->SetBrake(1.0f);
			m_bNeutral = false;
		}
	}
	if (activationMode == eAAM_OnRelease)
	{
		if (m_pVehiclePhysics)
		{
			m_pVehiclePhysics->SetThrottle(0.0f);
			m_bNeutral = true;
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
			m_pVehiclePhysics->SetThrottle(value);
			m_bNeutral = false;
		}
		else
		{
			m_pVehiclePhysics->SetThrottle(0.0f);
			m_bNeutral = true;
		}
	}
}