// Deer Solar Games | 2021 | Lighthouse project

// Camera controller component for game view
// Writed by quantbrain

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntitySystem.h>

#include <DefaultComponents/Cameras/CameraComponent.h>

#include "Framework/PlayerInput.h"
#include "Framework/Events/PawnEvents.h"

class CCameraInputEvents : public IInputEvents
{
public:
	CCameraInputEvents()
	{
		ObserverManager::subscribe<IInputEvents>(this);
	}

	~CCameraInputEvents()
	{
		ObserverManager::unsubscribe<IInputEvents>(this);
	}

	//IInputEvents
	virtual void OnMouseX(int activationMode, float value) override {};
	virtual void OnMouseY(int activationMode, float value) override {};

	virtual void OnYawDeltaXIRight(int activationMode, float value) override {};
	virtual void OnPitchDeltaXIRight(int activationMode, float value) override {};

	virtual void OnMouseWheel(int activationMode, float value) override {};

	virtual void OnMouseButtonRight(int activationMode, float value) override {};
	virtual void OnTriggerXIRight(int activationMode, float value) override {};
	//~IInputEvents
};

class CCameraPawnEvents : public GameEvents::IPawnEvents
{
public:
	CCameraPawnEvents()
	{
		GameEvents::CPawnEvents::Get()->Subscribe(this);
	}

	~CCameraPawnEvents()
	{
		GameEvents::CPawnEvents::Get()->Unsubscribe(this);
	}

	//IPawnEvents
	virtual void OnPawnStartFight() override {};
	virtual void OnPawnReleaseFight() override {};
	virtual void OnPawnSetDriving() override {};
	virtual void OnPawnReleaseDriving() override {};
	//~IPawnEvents
};

class CCameraControllerComponent final : public IEntityComponent, public CCameraInputEvents, public CCameraPawnEvents
{
public:
	// SIGNALS
	struct SPressSecondaryInteractSignal
	{
		SPressSecondaryInteractSignal() = default;
	};

	struct SReleaseSecondaryInteractSignal
	{
		SReleaseSecondaryInteractSignal() = default;
	};

	struct SPawnStartFightSignal
	{
		SPawnStartFightSignal() = default;
	};

	struct SPawnReleaseFightSignal
	{
		SPawnReleaseFightSignal() = default;
	};

	struct SPawnSetDrivingSignal
	{
		SPawnSetDrivingSignal() = default;
	};

	struct SPawnReleaseDrivingSignal
	{
		SPawnReleaseDrivingSignal() = default;
	};
	//~SIGNALS

	CCameraControllerComponent();
	~CCameraControllerComponent();

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
		desc.SetComponentFlags({ IEntityComponent::EFlags::Singleton, IEntityComponent::EFlags::HideFromInspector });

		//Common
		desc.AddMember(&CCameraControllerComponent::m_cameraFarPlane, 'fpcm', "CameraFarPlane", "Camera Far Plane", "..", 8000.0f);

		//Cinematic
		desc.AddMember(&CCameraControllerComponent::followMoveCameraSpeed, 'fmcs', "FollowMoveCameraSpeed", "Follow Move Camera Speed", "..", 1.0f);
		desc.AddMember(&CCameraControllerComponent::followRotateCameraSpeed, 'frcs', "FollowRotateCameraSpeed", "Follow Rotate Camera Speed", "..", 1.0f);
		desc.AddMember(&CCameraControllerComponent::m_cameraRootName, 'rtne', "CameraRootName", "Camera Root Name", "For Cinematic mode", "_root");

		desc.AddMember(&CCameraControllerComponent::m_isLookAt, 'lkat', "IsLookAtPawn", "IsLookAtPawn", "IsLookAtPawn", false);
		desc.AddMember(&CCameraControllerComponent::m_isViewControl, 'vctr', "IsViewControl", "IsViewControl", "IsViewControl", false);

		//Third Person
		desc.AddMember(&CCameraControllerComponent::m_newCameraDistanceThirdPerson, 'dsnt', "CameraDistanceThirdPerson", "Camera Distance Third Person", "..", 3.f);
		desc.AddMember(&CCameraControllerComponent::m_pitchMinThirdPerson, 'pmax', "PitchMinThirdPerson", "Pitch Min Third Person", "..", 15.f);
		desc.AddMember(&CCameraControllerComponent::m_pitchMaxThirdPerson, 'pmin', "PitchMaxThirdPerson", "Pitch Max Third Person", "..", 85.f);

		desc.AddMember(&CCameraControllerComponent::m_newRightOffsetThirdPerson, 'rifs', "RightOffsetThirdPerson", "Right Offset Third Person", "..", 0.f);

		desc.AddMember(&CCameraControllerComponent::m_fieldOfViewDefault, 'fovd', "FoVDefault", "FoV Default", "..", 65.f);
		desc.AddMember(&CCameraControllerComponent::m_fieldOfViewExamine, 'fove', "FoVExamine", "FoV Examine", "..", 45.f);
		desc.AddMember(&CCameraControllerComponent::m_fieldOfViewAiming, 'fova', "FoVAiming", "FoV Aiming", "..", 20.f);

		//Vehicle
		desc.AddMember(&CCameraControllerComponent::m_newCameraDistanceVehicle, 'cdvl', "CameraDistanceVehicle", "Camera Distance Vehicle", "..", 3.f);
		desc.AddMember(&CCameraControllerComponent::m_pitchMinVehicle, 'pmxv', "PitchMinVehicle", "Pitch Min Vehicle", "..", 15.f);
		desc.AddMember(&CCameraControllerComponent::m_pitchMaxVehicle, 'pmnv', "PitchMaxVehicle", "Pitch Max Vehicle", "..", 85.f);

		desc.AddMember(&CCameraControllerComponent::m_newRightOffsetVehicle, 'rovl', "RightOffsetVehicle", "Right Offset Vehicle", "..", 0.f);
	}

	static CCameraControllerComponent* Get() { return m_pInstance; }

	void UpdateThirdPerson();
	void UpdateCinematic();
	void UpdateVehicle();

	void UpdateInputs();

	void SetFoV(float fov);
	void SetRightOffsetTP(float val);
	void SetCameraDistanceTP(float val);

private:
	void CameraZoom(const float& value);
	Vec3 GetViewDir() { return m_pCameraComponent->GetCamera().GetViewdir(); }

	bool CollisionDetection(const Vec3& TargetPosition, Vec3& CameraPosition);

private:
	Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;

	//Common
	float m_cameraFarPlane = 8000.0f;

	//Third Person
	float m_cameraDistanceThirdPerson = 3.f;
	float m_newCameraDistanceThirdPerson;
	float m_saveCameraDistanceThirdPerson;

	Schematyc::Range<0, 89> m_pitchMinThirdPerson = 15.0f;
	Schematyc::Range<0, 89> m_pitchMaxThirdPerson = 85.0f;

	float m_rightOffsetThirdPerson;
	float m_newRightOffsetThirdPerson;

	//Vehicle
	float m_cameraDistanceVehicle = 3.f;
	float m_newCameraDistanceVehicle;
	float m_saveCameraDistanceVehicle;

	Schematyc::Range<0, 89> m_pitchMinVehicle = 15.0f;
	Schematyc::Range<0, 89> m_pitchMaxVehicle = 85.0f;

	float m_rightOffsetVehicle;
	float m_newRightOffsetVehicle;
	//~Vehicle

	float m_viewPitch = DEG2RAD(45.f);
	float m_viewYaw;

	Quat m_quatLastTargetRotation{ IDENTITY };
	Quat m_quatLastViewRotation{ IDENTITY };
	Quat m_quatTargetRotation{ IDENTITY };
	Vec3 m_vecLastPosition{ ZERO };

	//FOV
	float m_fieldOfView = 65.0f;
	float m_fieldOfViewDefault = 65.0f;
	float m_fieldOfViewExamine = 45.0f;
	float m_fieldOfViewAiming = 15.0f;
	float m_newFieldOfView;

	//Cinematic
	float followMoveCameraSpeed = 1.0f;
	float followRotateCameraSpeed = 1.0f;

	bool m_isLookAt = false;
	bool m_isViewControl = false;

	Schematyc::CSharedString m_cameraRootName = "_root";
	IEntity* m_pCameraRootEntity{ nullptr };
	//~Cinematic

	static CCameraControllerComponent* m_pInstance;

private:
	Ang3 GetRotationDelta();

	float GetPitchDelta();

	float GetYawDelta();

	float GetMousePitchDelta();

	float GetMouseYawDelta();

	float GetXiPitchDelta();

	float GetXiYawDelta();

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

private:
	//IInputEvents
	virtual void OnMouseX(int activationMode, float value) override;
	virtual void OnMouseY(int activationMode, float value) override;

	virtual void OnYawDeltaXIRight(int activationMode, float value) override;
	virtual void OnPitchDeltaXIRight(int activationMode, float value) override;

	virtual void OnMouseWheel(int activationMode, float value) override;

	virtual void OnMouseButtonRight(int activationMode, float value) override;
	virtual void OnTriggerXIRight(int activationMode, float value) override;
	//~IInputEvents

	//IPawnEvents
	virtual void OnPawnStartFight() override;
	virtual void OnPawnReleaseFight() override;
	virtual void OnPawnSetDriving() override;
	virtual void OnPawnReleaseDriving() override;
	//~IPawnEvents
};
