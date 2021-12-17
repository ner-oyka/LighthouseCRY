//Deer Solar Games | 2021 | Lighthouse project

#pragma once

#include <CryEntitySystem/IEntitySystem.h>

#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>
#include <CryAISystem/Components/IEntityBehaviorTreeComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

#include <DefaultComponents/Lights/ProjectorLightComponent.h>

#include "Framework/Events/PawnEvents.h"

class CAssistantComponent final : public IEntityComponent, public GameEvents::IPawnEvents
{
public:
	CAssistantComponent() = default;
	virtual ~CAssistantComponent();

	virtual void Initialize() override;
	virtual ComponentEventPriority GetEventPriority() const override final;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	static void ReflectType(Schematyc::CTypeDesc<CAssistantComponent>& desc)
	{
		desc.SetGUID("{3EF281CE-84B4-48A7-8FB1-13D3BB7DA2E4}"_cry_guid);
		desc.SetEditorCategory("Game");
		desc.SetLabel("Assistant");
		desc.SetDescription("This is automatic mobile science station");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });

		desc.AddMember(&CAssistantComponent::m_flashlightClass, 'flcl', "FlashlightEntityClass", "FlashlightEntityClass", "FlashlightEntityClass.", "");
	}
	Schematyc::EntityClassName m_flashlightClass = "";

	//Send BevaviorTree SIGNALS
	void SendEventBehaviorTree(string signalName);

	bool MoveToPlayer();

	bool FollowToPlayer();

	bool Searching(Vec3 searchPos);

	bool MoveToLocation(Vec3 targetPos);

	Vec3 FindReachablePosition(Vec3 position);

	void SendDRSSignal(string signalName);

	float GetDistanceToPlayer();

private:
	void UpdateLook();

	void InitializeEngineBones();
	void UpdateEngineDir();

	void UpdateCheckCurrentPosition();

	Quat RandomLook();

	void CreateFlashlight();

private:
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
	IEntityNavigationComponent* m_pNavigationAgent = nullptr;
	IEntityBehaviorTreeComponent* m_pBehaviorTreeComponent = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationMeshComponent = nullptr;


private:
	IEntity* m_playerEntity;

	IEntity* m_pFlashlightEntity = nullptr;


	//DRS
	DRS::IResponseActor* p_DRSActor = nullptr;

	Vec3 m_currentVelocity = ZERO;

	//Searching
	float m_searchNewPosDuration = 2.0f;

	//Look
	bool m_bRandomLook = false;
	float m_globalLookDuration = 2.f;
	float m_localLookDuration = 0.3f;

	float m_currentGlobalLookTime;
	float m_currentLocalLookTime;

	Quat m_currentRandomLookRotation{ IDENTITY };

	IAnimationOperatorQueuePtr m_lookAtModifier;

	//Rotate Engines
	IAnimationOperatorQueuePtr m_engineRotateAtModifier;
	Vec3 m_originalRightEnginePos = ZERO;
	Vec3 m_originalLeftEnginePos = ZERO;

	int m_engineRightBoneId = 0;
	int m_engineLeftBoneId = 0;

	//Check stuck
	Vec3 m_lastReachablePosition = ZERO;

private:
	//Pawn Events
	virtual void OnStartSelectTargetForAssistant();
	//Pawn Events
};