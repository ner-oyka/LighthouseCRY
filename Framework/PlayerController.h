//Deer Solar Games | 2021 | Lighthouse project

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/ResourceTypes.h>
#include <DefaultComponents/Cameras/CameraComponent.h>
#include "IPawn.h"
#include "UIManager.h"

#include <FlashUI/FlashUI.h>

class CPlayerController final : public IEntityComponent
{
public:
	enum class EPawnSwitchingTypes
	{
		Cut,
		Lerp,
		Animation
	};

	CPlayerController() = default;
	virtual ~CPlayerController() override final { if (m_pInstance == this) m_pInstance = nullptr; };

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	static void ReflectType(Schematyc::CTypeDesc<CPlayerController>& desc)
	{
		desc.SetGUID("{7CA0C637-9AEC-4D7D-84C5-C288B6CAEA65}"_cry_guid);
		desc.SetComponentFlags({ IEntityComponent::EFlags::Singleton, IEntityComponent::EFlags::HideFromInspector });
		desc.AddMember(&CPlayerController::m_alexPlayerClass, 'apcl', "AlexPlayerClass", "Alex Player Class", "Alex player class.", "");
		desc.AddMember(&CPlayerController::m_assistantClass, 'ascl', "AssistantEntity", "Assistant Entity", "Assistant Entity.", "");
	}

	Schematyc::EntityClassName m_alexPlayerClass = "";
	Schematyc::EntityClassName m_assistantClass = "";

	void AlexSpawn();
	void AssistantSpawn();

	static CPlayerController* Get() { return m_pInstance; }

	IPawn* GetControlledPawn() { return m_pControlledPawn; }

	IEntity* GetAssistant() { return m_pAssistant; }

	void SetControlledPawn(IPawn* pawn, EPawnSwitchingTypes actorSwitchingType = EPawnSwitchingTypes::Cut)
	{
		if (pawn && pawn == m_pControlledPawn)
			return;

		if (m_pControlledPawn)
		{
			ObserverManager::unsubscribe(m_pControlledPawn);
		}

		m_pControlledPawn = pawn;
		ObserverManager::subscribe<IInputEvents>(m_pControlledPawn);
	}

private:
	IPawn* m_pControlledPawn = nullptr;
	IEntity* m_pAssistant = nullptr;

	CUIManager* m_pUIManager = nullptr;

	static CPlayerController* m_pInstance;
};
