#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/ResourceTypes.h>

class CWeaponComponent final : public IEntityComponent
{
	struct SDefault
	{
		static constexpr bool  startEnabled = true;
		static constexpr float fireSpeed = 5.f;
	};

public:
	struct SStartShotSignal
	{
		SStartShotSignal() = default;
	};

	struct SStopShotSignal
	{
		SStopShotSignal() = default;
	};

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CWeaponComponent>& desc)
	{
		desc.SetLabel("Weapon");
		desc.SetEditorCategory("Weapons");
		desc.SetDescription("A weapon component that can fire specified projectiles.");
		desc.SetGUID("{18C8C211-46FB-4C87-B92C-0396019B0B79}"_cry_guid);
		desc.SetComponentFlags({ IEntityComponent::EFlags::Singleton, IEntityComponent::EFlags::HideFromInspector });

		desc.AddMember(&CWeaponComponent::m_startEnabled, 'sen', "StartEnabled", "Start Enabled", "If set, the weapon will be enabled when it is spawned.", SDefault::startEnabled);
		desc.AddMember(&CWeaponComponent::m_fireSpeed, 'fspd', "FireSpeed", "Fire Speed", "Number of projectiles fired form this weapon per second", SDefault::fireSpeed);
		desc.AddMember(&CWeaponComponent::m_projectileClassname, 'pcls', "ProjectileClass", "Projectile Class", "The entity class that represents this weapons projectile entity", "");
	}

	CWeaponComponent() = default;
	~CWeaponComponent() override final;

	void StartShootEvent();
	void StopShootEvent();

	void SpawnProjectile();

protected:
	// IEntityComponent
	virtual void Initialize() override final;
	virtual EntityEventMask GetEventMask() const override final;
	virtual ComponentEventPriority GetEventPriority() const override final;
	virtual void ProcessEvent(const SEntityEvent& event) override final;
	// ~IEntityComponent

private:
	bool m_weaponsEnabled = true;
	float m_timeSinceLastProjectile = 0.f;

	bool m_startEnabled = SDefault::startEnabled;
	float m_timeBetweenProjectiles = 1 / SDefault::fireSpeed;
	float m_fireSpeed = SDefault::fireSpeed;
	Schematyc::EntityClassName m_projectileClassname = "";
};