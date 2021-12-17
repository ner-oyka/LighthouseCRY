//Deer Solar Games | 2021 | Lighthouse project

#pragma once

#include <DefaultComponents/Physics/Vehicles/VehicleComponent.h>
#include <DefaultComponents/Physics/Vehicles/WheelComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

#include "Framework/IPawn.h"

class CCarPlayer final : public IPawn
{
public:
	CCarPlayer() = default;
	virtual ~CCarPlayer() override final;

	// IEntityComponent
	virtual void Initialize() override;
	virtual ComponentEventPriority GetEventPriority() const override final;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent & event) override;
	// ~IEntityComponent

	static void ReflectType(Schematyc::CTypeDesc<CCarPlayer>&desc)
	{
		desc.SetLabel("Car Controller");
		desc.SetEditorCategory("Vehicles");
		desc.SetDescription("Car controller component.");
		desc.SetGUID("{94858DE8-4C9B-4B35-9DD7-BDC093AC1ABE}"_cry_guid);
		desc.SetComponentFlags({ IEntityComponent::EFlags::Singleton });
	}

	int GetCurrentGear();
	float GetEngineRPM();
	float GetThrottle();
	float GetFootBrake();

	void SetDriver(IEntity* driverEntity);
	void ReleaseDriver();

private:
	void InitializeWheels();
	void UpdateWheels();

private:
	//IInputEvents
	virtual void OnForward(int activationMode, float value) override;
	virtual void OnBackward(int activationMode, float value) override;
	virtual void OnLeft(int activationMode, float value) override;
	virtual void OnRight(int activationMode, float value) override;

	virtual void OnYawDeltaXILeft(int activationMode, float value) override;
	virtual void OnPitchDeltaXILeft(int activationMode, float value) override;

	virtual void OnMouseX(int activationMode, float value) override;
	virtual void OnMouseY(int activationMode, float value) override;
	virtual void OnYawDeltaXIRight(int activationMode, float value) override;
	virtual void OnPitchDeltaXIRight(int activationMode, float value) override;

	virtual void OnEnter(int activationMode, float value) override;

	virtual void OnTriggerXIRight(int activationMode, float value) override;
	//~IInputEvents

private:
	Cry::DefaultComponents::CVehiclePhysicsComponent* m_pVehiclePhysics = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;

	IEntity* m_pCarDriverEntity = nullptr;

	DynArray<Cry::DefaultComponents::CWheelComponent*> m_pWheelsComponents;

	//Animate wheels
	IAnimationOperatorQueuePtr m_poseWheelsModifier;

	bool m_bNeutral = false;
};