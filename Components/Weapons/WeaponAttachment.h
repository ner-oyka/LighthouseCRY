#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/ResourceTypes.h>

class CWeaponAttachementComponent final : public IEntityComponent
{
public:
	static void ReflectType(Schematyc::CTypeDesc<CWeaponAttachementComponent>& desc)
	{
		desc.SetLabel("Weapon Attachment");
		desc.SetEditorCategory("Weapons");
		desc.SetDescription("This weapon attachment component can hold a weapon, it can also exchange weapons (via entity classes).");
		desc.SetGUID("{8D1DFA29-32B3-46E7-A471-70AEF48A84D6}"_cry_guid);
		desc.SetComponentFlags({ IEntityComponent::EFlags::Singleton, IEntityComponent::EFlags::HideFromInspector });

		desc.AddMember(&CWeaponAttachementComponent::m_currentWeapon, 'cwep', "CurrentWeapon", "Current Weapon", "The current weapon this weapon attachment is holding.", "");
	}

	CWeaponAttachementComponent() = default;
	~CWeaponAttachementComponent() override final;

	void RemoveCurrentWeapon();
	void SpawnWeaponEntity();

	void StartShoot();
	void StopShoot();

protected:
	// IEntityComponent
	virtual void Initialize() override final;
	virtual EntityEventMask GetEventMask() const override final;
	virtual void ProcessEvent(const SEntityEvent& event) override final;
	// ~IEntityComponent

private:


	Schematyc::EntityClassName m_currentWeapon = "";
	EntityId m_weaponEntityId = INVALID_ENTITYID;
};