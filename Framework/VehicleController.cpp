#include "StdAfx.h"
#include "VehicleController.h"

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>

static void RegisterVehicleControllerComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope entityScope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = entityScope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CVehicleController));

		// Functions
		{
		}
		// Signals
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterVehicleControllerComponent)

void CVehicleController::Initialize()
{
	m_pVehiclePhysics = m_pEntity->GetComponent<Cry::DefaultComponents::CVehiclePhysicsComponent>();
	m_pAnimationComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	InitializeWheels();

	CryCreateClassInstanceForInterface(cryiidof<IAnimationOperatorQueue>(), m_poseWheelsModifier);
}

Cry::Entity::EventFlags CVehicleController::GetEventMask() const
{
	return Cry::Entity::EEvent::GameplayStarted | Cry::Entity::EEvent::PrePhysicsUpdate | Cry::Entity::EEvent::EditorPropertyChanged;
}

void CVehicleController::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case EEntityEvent::GameplayStarted:
	{
		InitializeWheels();
	}
	break;
	case EEntityEvent::PrePhysicsUpdate:
	{
		UpdateWheels();
	}
	break;
	case EEntityEvent::EditorPropertyChanged:
	{
		InitializeWheels();
	}
	break;
	}
}

Vec3 CVehicleController::GetVelocity()
{
	if (m_pVehiclePhysics)
	{
		return m_pVehiclePhysics->GetVelocity();
	}
	return ZERO;
}

void CVehicleController::SetVelocity(const Vec3 &vel)
{
	if (m_pVehiclePhysics)
	{
		m_pVehiclePhysics->SetVelocity(vel);
	}
}

void CVehicleController::SetThrottle(float value)
{
	if (m_pVehiclePhysics)
	{
		m_pVehiclePhysics->SetThrottle(value);
	}
}

void CVehicleController::SetCurrentGear(int gearId)
{
	if (m_pVehiclePhysics)
	{
		m_pVehiclePhysics->SetCurrentGear(gearId);
	}
}

void CVehicleController::SetBrake(float value)
{
	if (m_pVehiclePhysics)
	{
		m_pVehiclePhysics->SetBrake(value);
	}
}

void CVehicleController::SetSteeringAngle(float deg)
{
	if (m_pVehiclePhysics)
	{
		m_pVehiclePhysics->SetSteeringAngle(CryTransform::CAngle::FromDegrees(deg));
	}
}

void CVehicleController::InitializeWheels()
{
	if (m_axleInfo.m_axleParams.Size() == 0)
	{
		CryLog("Wheel parameters are not configured!");
		return;
	}

	GetEntity()->RemoveAllComponents<Cry::DefaultComponents::CWheelComponent>();
	m_wheelsInfo.clear();

	for (int i = 0; i < m_axleInfo.m_axleParams.Size(); i++)
	{
		if (m_axleInfo.m_axleParams.At(i).m_isOneWheelPerAxle)
		{
			//configurable for one wheel on axle
			return;
		}
		else
		{
			Cry::DefaultComponents::CWheelComponent* leftWheelComponent = GetEntity()->CreateComponent<Cry::DefaultComponents::CWheelComponent>();
			Cry::DefaultComponents::CWheelComponent* rightWheelComponent = GetEntity()->CreateComponent<Cry::DefaultComponents::CWheelComponent>();

			//for left wheel
			leftWheelComponent->SetAxleIndex(m_axleInfo.m_axleParams.At(i).m_axleIndexLeft);
			leftWheelComponent->m_radius = m_axleInfo.m_axleParams.At(i).m_leftWheelRadius;
			leftWheelComponent->m_height = m_axleInfo.m_axleParams.At(i).m_leftWheelWidth;
			leftWheelComponent->m_driving = m_axleInfo.m_axleParams.At(i).m_leftWheelDriving;
			leftWheelComponent->m_handBrake = m_axleInfo.m_axleParams.At(i).m_leftWheelHandbrake;
			leftWheelComponent->m_suspensionLength = m_axleInfo.m_axleParams.At(i).m_leftWheelSuspensionLength;
			leftWheelComponent->m_damping = m_axleInfo.m_axleParams.At(i).m_leftWheelSuspensionDamping;
	
			//for right wheel
			rightWheelComponent->SetAxleIndex(m_axleInfo.m_axleParams.At(i).m_axleIndexRight);
			rightWheelComponent->m_radius = m_axleInfo.m_axleParams.At(i).m_rightWheelRadius;
			rightWheelComponent->m_height = m_axleInfo.m_axleParams.At(i).m_rightWheelWidth;
			rightWheelComponent->m_driving = m_axleInfo.m_axleParams.At(i).m_rightWheelDriving;
			rightWheelComponent->m_handBrake = m_axleInfo.m_axleParams.At(i).m_rightWheelHandbrake;
			rightWheelComponent->m_suspensionLength = m_axleInfo.m_axleParams.At(i).m_rightWheelSuspensionLength;
			rightWheelComponent->m_damping = m_axleInfo.m_axleParams.At(i).m_rightWheelSuspensionDamping;

			//set wheels to joint
			if (ICharacterInstance* pCharacter = m_pAnimationComponent->GetCharacter())
			{
				//left
				if (IAttachment* pAttachment = pCharacter->GetIAttachmentManager()->GetInterfaceByName(m_axleInfo.m_axleParams.At(i).m_jointNameLeftWheel.c_str()))
				{
					const QuatT rel = pAttachment->GetAttAbsoluteDefault();
					Matrix34 mat = Matrix34(rel);
					leftWheelComponent->SetTransformMatrix(mat);
				}
				//right
				if (IAttachment* pAttachment = pCharacter->GetIAttachmentManager()->GetInterfaceByName(m_axleInfo.m_axleParams.At(i).m_jointNameRightWheel.c_str()))
				{
					const QuatT rel = pAttachment->GetAttAbsoluteDefault();
					Matrix34 mat = Matrix34(rel);
					rightWheelComponent->SetTransformMatrix(mat);
				}
			}
			SWheelInfo wheelLeftInfo;
			wheelLeftInfo.m_pWheelComponent = leftWheelComponent;
			wheelLeftInfo.m_entitySlotId = m_pEntity->GetPhysicalEntityPartId0(leftWheelComponent->GetEntitySlotId());
			wheelLeftInfo.m_boneName = m_axleInfo.m_axleParams.At(i).m_boneNameLeftWheel.c_str();

			SWheelInfo wheelRightInfo;
			wheelRightInfo.m_pWheelComponent = rightWheelComponent;
			wheelRightInfo.m_entitySlotId = m_pEntity->GetPhysicalEntityPartId0(rightWheelComponent->GetEntitySlotId());
			wheelRightInfo.m_boneName = m_axleInfo.m_axleParams.At(i).m_boneNameRightWheel.c_str();

			m_wheelsInfo.push_back(wheelLeftInfo);
			m_wheelsInfo.push_back(wheelRightInfo);
		}
	}	
}

void CVehicleController::UpdateWheels()
{
	if (ICharacterInstance* pCharacterInstance = m_pAnimationComponent->GetCharacter())
	{
		pCharacterInstance->GetISkeletonAnim()->PushPoseModifier(2, m_poseWheelsModifier, "Pose Wheels");

		ISkeletonPose* pSkeleton = pCharacterInstance->GetISkeletonPose();
		IDefaultSkeleton& rIDefaultSkeleton = pCharacterInstance->GetIDefaultSkeleton();

		for (SWheelInfo wheel : m_wheelsInfo)
		{
			int m_wheelBoneId = rIDefaultSkeleton.GetJointIDByName(wheel.m_boneName);

			QuatT rel = pSkeleton->GetAbsJointByID(m_wheelBoneId);

			pe_status_wheel sw;
			pe_status_pos sp;
			sw.partid = wheel.m_entitySlotId;
			sp.partid = wheel.m_entitySlotId;

			m_pEntity->GetPhysicalEntity()->GetStatus(&sw);
			m_pEntity->GetPhysicalEntity()->GetStatus(&sp);

			rel.q = sp.q;
			rel.t = sp.pos;

			m_poseWheelsModifier->PushOrientation(m_wheelBoneId, IAnimationOperatorQueue::eOp_OverrideWorld, rel.q);
			m_poseWheelsModifier->PushPosition(m_wheelBoneId, IAnimationOperatorQueue::eOp_OverrideWorld, rel.t);
		}

	}
}
