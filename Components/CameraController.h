//Deer Solar Games | 2021 | Lighthouse project

//Camera controller component for game view
//Third Person Camera mode
//Cinematic mode: finding main cinemetic camera and replase its pos and rot for smooth animation and more features:
//LookAt: look at player character
//ViewControl: rotate view camera as FPS
//TransformControl: rotate view camera around player character with adjusted rotate limited
//Default: cinematic mode used full features track view

//Writed by quantbrain

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntitySystem.h>

#include <DefaultComponents/Cameras/CameraComponent.h>

#include "Framework/PlayerInput.h"


namespace Game
{
	enum class ECameraType
	{
		Default = 0,
		LookAt = 1,
		ViewControl = 2,
		ThirdPerson = 3
	};

	static void ReflectType(Schematyc::CTypeDesc<ECameraType>& desc)
	{
		desc.SetGUID("{3622DAA9-FA64-40DF-9308-7B4BC1D107D8}"_cry_guid);
		desc.SetLabel("Camera Type");
		desc.SetDefaultValue(ECameraType::Default);
		desc.AddConstant(ECameraType::Default, "Default", "Default");
		desc.AddConstant(ECameraType::LookAt, "LookAt", "LookAt");
		desc.AddConstant(ECameraType::ViewControl, "ViewControl", "ViewControl");
		desc.AddConstant(ECameraType::ThirdPerson, "ThirdPerson", "ThirdPerson");
	}
}

class CCameraControllerComponent final : public IEntityComponent, public IInputEvents
{
	struct SCameraOffsetTP
	{
		static constexpr float cameraOffset[] = { 0.f, -7.f, 0.f };
	};

public:
	CCameraControllerComponent() { ObserverManager::subscribe<IInputEvents>(this); }
	~CCameraControllerComponent() { ObserverManager::unsubscribe(this); }

	// IEntityComponent
	virtual void Initialize() override;
	virtual EntityEventMask GetEventMask() const override final;
	virtual ComponentEventPriority GetEventPriority() const override final;
	virtual void ProcessEvent(const SEntityEvent& event) override final;
	// ~IEntityComponent

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CCameraControllerComponent>& desc)
	{
		desc.SetGUID("{22A6DE22-48A3-487E-8D45-C8FB631C64A5}"_cry_guid);
		desc.SetLabel("Camera Controller");
		desc.SetEditorCategory("Camera");
		desc.SetDescription("The camera controller component.");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Singleton });

		desc.AddMember(&CCameraControllerComponent::cameraType, 'type', "CameraType", "Camera Type", "..", Game::ECameraType::Default);
		desc.AddMember(&CCameraControllerComponent::followMoveCameraSpeed, 'fmcs', "FollowMoveCameraSpeed", "Follow Move Camera Speed", "..", 1.0f);
		desc.AddMember(&CCameraControllerComponent::followRotateCameraSpeed, 'frcs', "FollowRotateCameraSpeed", "Follow Rotate Camera Speed", "..", 1.0f);
		desc.AddMember(&CCameraControllerComponent::m_cameraRootName, 'rtne', "CameraRootName", "Camera Root Name", "..", "_root");

		//Third Person
		desc.AddMember(&CCameraControllerComponent::m_cameraOffsetThirdPerson, 'cost', "CameraOffset", "Camera Offset", "..", Vec3(SCameraOffsetTP::cameraOffset[0], SCameraOffsetTP::cameraOffset[1], SCameraOffsetTP::cameraOffset[2]));
		desc.AddMember(&CCameraControllerComponent::m_cameraDistanceThirdPerson, 'dsnt', "CameraDistance", "Camera Distance", "..", 3.f);
		desc.AddMember(&CCameraControllerComponent::m_pitchMin, 'pmax', "PitchMin", "Pitch Min", "..", 15.f);
		desc.AddMember(&CCameraControllerComponent::m_pitchMax, 'pmin', "PitchMax", "Pitch Max", "..", 85.f);
	}

private:
	void UpdateCamera();
	void UpdateInputs();

	void SetOffset(const Vec3& offset);
	void CameraZoom(const float& value);
	Vec3 GetOffset() { return m_cameraOffsetThirdPerson; }
	Vec3 GetViewDir() { return m_pCameraComponent->GetCamera().GetViewdir(); }

	bool CollisionDetection(const Vec3& TargetPosition, Vec3& CameraPosition);

protected:
	Game::ECameraType cameraType = Game::ECameraType::Default;

private:
	Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;


	float m_cameraDistanceThirdPerson = 3.f;
	float m_newCameraDistanceThirdPerson;

	Vec3 m_cameraOffsetThirdPerson = Vec3(SCameraOffsetTP::cameraOffset[0], SCameraOffsetTP::cameraOffset[1], SCameraOffsetTP::cameraOffset[2]);

	float m_viewPitch = DEG2RAD(45.f);
	float m_viewYaw;

	Quat m_quatLastTargetRotation{ IDENTITY };
	Quat m_quatLastViewRotation{ IDENTITY };
	Quat m_quatTargetRotation{ IDENTITY };
	Vec3 m_vecLastPosition{ ZERO };

	Schematyc::Range<0, 89> m_pitchMin = 15.0f;
	Schematyc::Range<0, 89> m_pitchMax = 85.0f;

	Schematyc::Range<0, 32768> m_nearPlane = 0.25f;
	CryTransform::CClampedAngle<20, 360> m_fieldOfView = 55.0_degrees;

	float followMoveCameraSpeed = 1.0f;
	float followRotateCameraSpeed = 1.0f;

	Schematyc::CSharedString m_cameraRootName = "_root";
	IEntity* m_pCameraRootEntity{ nullptr };

private:
	Ang3 GetRotationDelta();

	float GetPitchDelta();

	float GetYawDelta();

	float GetMousePitchDelta();

	float GetMouseYawDelta();

	float GetXiPitchDelta();

	float GetXiYawDelta();

	//IInputEvents
	virtual void OnMouseX(int activationMode, float value) override;
	virtual void OnMouseY(int activationMode, float value) override;

	virtual void OnYawDeltaXIRight(int activationMode, float value) override;
	virtual void OnPitchDeltaXIRight(int activationMode, float value) override;

	virtual void OnMouseWheel(int activationMode, float value) override;
	//~IInputEvents

private:
	//Inputs
	int m_mouseInvertPitch{ 0 };

	float m_mousePitchYawSensitivity{ 1.0f };

	float m_lastPitchDelta{ 0.0f };

	float m_lastYawDelta{ 0.0f };

	float m_mousePitchDelta{ 0.0f };
	float m_lastMousePitchDelta{ 0.0f };

	float m_mouseYawDelta{ 0.0f };
	float m_lastMouseYawDelta{ 0.0f };

	float m_xiPitchDelta{ 0.0f };
	float m_lastXiPitchDelta{ 0.0f };

	float m_xiYawDelta{ 0.0f };
	float m_lastXiYawDelta{ 0.0f };

	float m_pitchFilter{ 0.0001f };

	float m_yawFilter{ 0.0001f };

	float m_xiPitchFilter{ 0.0001f };

	float m_xiYawFilter{ 0.0001f };
};
