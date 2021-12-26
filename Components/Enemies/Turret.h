#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/ResourceTypes.h>

class CTurretComponent final : public IEntityComponent
{
public:

	static void ReflectType(Schematyc::CTypeDesc<CTurretComponent>& desc)
	{
		desc.SetLabel("Turret component");
		desc.SetEditorCategory("Enemies");
		desc.SetDescription("Turret");
		desc.SetGUID("{FE1BAE70-55FF-4B9A-BAEE-D09D8F3532D5}"_cry_guid);
		desc.SetComponentFlags({ IEntityComponent::EFlags::Singleton, IEntityComponent::EFlags::HideFromInspector });

		desc.AddMember(&CTurretComponent::m_projectileClassname, 'pcls', "ProjectileClass", "Projectile Class", "The entity class that represents this weapons projectile entity", "");
	}

	CTurretComponent() = default;
	~CTurretComponent() override final;

	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	void Shot();

private:
	Schematyc::EntityClassName m_projectileClassname = "";
};
