#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/ResourceTypes.h>

#include <DefaultComponents/Physics/Vehicles/VehicleComponent.h>
#include <DefaultComponents/Physics/Vehicles/WheelComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

class CVehicleController final : public IEntityComponent
{
public:
	struct SWheelInfo
	{
		Cry::DefaultComponents::CWheelComponent* m_pWheelComponent = nullptr;
		int m_entitySlotId = 0;
		string m_boneName = "";
	};

	struct SAxleInfo
	{
		inline bool operator==(const SAxleInfo& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }

		struct SAxle
		{
			inline bool operator==(const SAxle& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
			inline bool operator!=(const SAxle& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

			void Serialize(Serialization::IArchive& archive)
			{
				archive(m_jointNameLeftWheel, "JointLeftWheel", "JointLeftWheel");
				archive(m_jointNameRightWheel, "JointRightWheel", "JointRightWheel");

				archive(m_boneNameLeftWheel, "BoneNameLeftWheel", "BoneNameLeftWheel");
				archive(m_boneNameRightWheel, "BoneNameRightWheel", "BoneNameRightWheel");

				archive(m_axleIndexLeft, "AxleIndexLeft", "AxleIndexLeft");
				archive(m_axleIndexRight, "AxleIndexRight", "AxleIndexRight");

				archive(m_leftWheelRadius, "LeftWheelRadius", "LeftWheelRadius");
				archive(m_rightWheelRadius, "RightWheelRadius", "RightWheelRadius");

				archive(m_leftWheelWidth, "LeftWheelWidth", "LeftWheelWidth");
				archive(m_rightWheelWidth, "RightWheelWidth", "RightWheelWidth");

				archive(m_leftWheelMass, "LeftWheelMass", "LeftWheelMass");
				archive(m_rightWheelMass, "RightWheelMass", "RightWheelMass");

				archive(m_leftWheelDriving, "LeftWheelDriving", "LeftWheelDriving");
				archive(m_rightWheelDriving, "RightWheelDriving", "RightWheelDriving");

				archive(m_leftWheelHandbrake, "LeftWheelHandbrake", "LeftWheelHandbrake");
				archive(m_rightWheelHandbrake, "RightWheelHandbrake", "RightWheelHandbrake");

				archive(m_leftWheelSuspensionLength, "LeftWheelSuspensionLength", "LeftWheelSuspensionLength");
				archive(m_rightWheelSuspensionLength, "RightWheelSuspensionLength", "RightWheelSuspensionLength");

				archive(m_leftWheelSuspensionDamping, "LeftWheelSuspensionDamping", "LeftWheelSuspensionDamping");
				archive(m_rightWheelSuspensionDamping, "RightWheelSuspensionDamping", "RightWheelSuspensionDamping");

				archive(m_isOneWheelPerAxle, "IsOneWheelPerAxle", "IsOneWheelPerAxle");
			}

			static void ReflectType(Schematyc::CTypeDesc<SAxle>& desc)
			{
				desc.SetGUID("{D0C1DF35-234E-460D-9E93-F27BDFA694C5}"_cry_guid);
				desc.SetLabel("Axis");
				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_jointNameLeftWheel, 'jnlw', "JointLeftWheel", "Joint Left Wheel", "...", "");
				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_jointNameRightWheel, 'jnrw', "JointRightWheel", "Joint Right Wheel", "...", "");

				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_boneNameLeftWheel, 'bnlw', "BoneNameLeftWheel", "Bone Name Left Wheel", "...", "");
				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_boneNameRightWheel, 'bnrw', "BoneNameRightWheel", "Bone Name Right Wheel", "...", "");

				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_axleIndexLeft, 'ailt', "AxleIndexLeft", "Axle Index Left", "...", 0);
				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_axleIndexRight, 'airt', "AxleIndexRight", "Axle Index Right", "...", 1);

				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_leftWheelRadius, 'lwrd', "LeftWheelRadius", "Left Wheel Radius", "...", 1);
				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_rightWheelRadius, 'rwrd', "RightWheelRadius", "Right Wheel Radius", "...", 1);

				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_leftWheelWidth, 'lwwd', "LeftWheelWidth", "Left Wheel Width", "...", 1);
				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_rightWheelWidth, 'rwwd', "RightWheelWidth", "Right Wheel Width", "...", 1);

				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_leftWheelMass, 'lwms', "LeftWheelMass", "Left Wheel Mass", "...", 1);
				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_rightWheelMass, 'rwms', "RightWheelMass", "Right Wheel Mass", "...", 1);

				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_leftWheelDriving, 'lwdr', "LeftWheelDriving", "Left Wheel Driving", "...", false);
				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_rightWheelDriving, 'rwdr', "RightWheelDriving", "Right Wheel Driving", "...", false);

				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_leftWheelHandbrake, 'lwhb', "LeftWheelHandbrake", "Left Wheel Handbrake", "...", false);
				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_rightWheelHandbrake, 'rwhb', "RightWheelHandbrake", "Right Wheel Handbrake", "...", false);

				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_leftWheelSuspensionLength, 'lwsp', "LeftWheelSuspensionLength", "Left Wheel Suspension Length", "...", 1);
				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_rightWheelSuspensionLength, 'rwsl', "RightWheelSuspensionLength", "Right Wheel Suspension Length", "...", 1);

				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_leftWheelSuspensionDamping, 'lwsd', "LeftWheelSuspensionDamping", "Left Wheel Suspension Damping", "...", 0.5f);
				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_rightWheelSuspensionDamping, 'rwsd', "RightWheelSuspensionDamping", "Right Wheel Suspension Damping", "...", 0.5f);

				desc.AddMember(&CVehicleController::SAxleInfo::SAxle::m_isOneWheelPerAxle, 'owpa', "IsOneWheelPerAxle", "Is One Wheel Per Axle", "If enabled then used left bone names.", false);
			}

			Schematyc::CSharedString m_jointNameLeftWheel = "";
			Schematyc::CSharedString m_jointNameRightWheel = "";

			Schematyc::CSharedString m_boneNameLeftWheel = "";
			Schematyc::CSharedString m_boneNameRightWheel = "";

			int m_axleIndexLeft = 0;
			int m_axleIndexRight = 1;

			float m_leftWheelRadius = 1;
			float m_rightWheelRadius = 1;

			float m_leftWheelWidth = 1;
			float m_rightWheelWidth = 1;

			float m_leftWheelMass = 1;
			float m_rightWheelMass = 1;

			bool m_leftWheelDriving = false;
			bool m_rightWheelDriving = false;

			bool m_leftWheelHandbrake = false;
			bool m_rightWheelHandbrake = false;

			float m_leftWheelSuspensionLength = 1;
			float m_rightWheelSuspensionLength = 1;

			float m_leftWheelSuspensionDamping = 0.5f;
			float m_rightWheelSuspensionDamping = 0.5f;

			bool m_isOneWheelPerAxle = false;
		};

		static void ReflectType(Schematyc::CTypeDesc<SAxleInfo>& desc)
		{
			desc.SetGUID("{6B4EF0E5-A7BA-4DD9-81F8-F487B24AA6A9}"_cry_guid);
			desc.SetLabel("Axis Info");
			desc.AddMember(&CVehicleController::SAxleInfo::m_axleParams, 'axin', "AxisInfo", "AxisInfo", "...", Schematyc::CArray<CVehicleController::SAxleInfo::SAxle>());
		}

		Schematyc::CArray<SAxle> m_axleParams;
	};

	static void ReflectType(Schematyc::CTypeDesc<CVehicleController>& desc)
	{
		desc.SetLabel("Vehicle Controller");
		desc.SetEditorCategory("Game");
		desc.SetDescription("Vehicle controller component.");
		desc.SetGUID("{57245EE2-D09F-4E13-89AB-710AF4C10CE3}"_cry_guid);
		desc.SetComponentFlags({ IEntityComponent::EFlags::Singleton, IEntityComponent::EFlags::HideFromInspector });

		desc.AddMember(&CVehicleController::m_axleInfo, 'ainf', "AxisInfo", "Axis Info", "...", CVehicleController::SAxleInfo());
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	CVehicleController() = default;
	virtual ~CVehicleController() = default;

	Vec3 GetVelocity();

	void SetVelocity(const Vec3& vel);
	void SetThrottle(float value);
	void SetCurrentGear(int gearId);
	void SetBrake(float value);
	void SetSteeringAngle(float deg);

protected:
	SAxleInfo m_axleInfo;

private:
	void InitializeWheels();
	void UpdateWheels();

private:
	Cry::DefaultComponents::CVehiclePhysicsComponent* m_pVehiclePhysics = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	DynArray<SWheelInfo> m_wheelsInfo;

	//Animate wheels
	IAnimationOperatorQueuePtr m_poseWheelsModifier;
};
