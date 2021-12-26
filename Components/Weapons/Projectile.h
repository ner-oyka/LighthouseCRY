#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/ResourceTypes.h>

class CProjectileComponent final : public IEntityComponent
{
	struct SDefault
	{
		static constexpr bool autoImpulse = true;
		static constexpr float speed = 1.f;
		static constexpr uint32 maxNumReflections = 2;
	};

public:
	static void ReflectType(Schematyc::CTypeDesc<CProjectileComponent>& desc)
	{
		desc.SetLabel("Projectile");
		desc.SetEditorCategory("Weapons");
		desc.SetDescription("Declares this entity as a projectile that can be used by a weapon.");
		desc.SetGUID("{15036DAE-0D14-4A0F-B341-C51CACDD0A7D}"_cry_guid);
		desc.SetComponentFlags({ IEntityComponent::EFlags::Singleton, IEntityComponent::EFlags::HideFromInspector });

		desc.AddMember(&CProjectileComponent::m_autoImpulse, 'aimp', "AutomaticImpulse", "Automatic Impulse", "Applies impulse to this projectile automatically when it is created.", SDefault::autoImpulse);
		desc.AddMember(&CProjectileComponent::m_speed, 'spd', "ProjectileSpeed", "Projectile Speed", "Speed of the projectile", SDefault::speed);
		desc.AddMember(&CProjectileComponent::m_maxNumReflections, 'nref', "MaxNumReflections", "Max Reflection Count", "Number of times this projectile can be reflected off the side of the arena", SDefault::maxNumReflections);

		desc.AddMember(&CProjectileComponent::m_explosionClassName, 'expl', "Explosion", "Explosion", "Explosion", "");
	}

	CProjectileComponent() = default;
	~CProjectileComponent() override final;

protected:
	// IEntityComponent
	virtual void Initialize() override final;
	virtual EntityEventMask GetEventMask() const override final;
	virtual void ProcessEvent(const SEntityEvent& event) override final;
	// ~IEntityComponent

	void SpawnExplosion(const Vec3& pt, const Vec3& normal);

private:
	uint32 m_numTimesReflected = 0;

	bool m_autoImpulse = SDefault::autoImpulse;
	float m_speed = SDefault::speed;
	uint32 m_maxNumReflections = SDefault::maxNumReflections;

	Schematyc::EntityClassName m_explosionClassName = "";

	float m_lifeTime = 5.0f;
};