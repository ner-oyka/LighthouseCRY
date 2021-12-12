// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryEntitySystem/IEntitySystem.h>

#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>
#include <CryAISystem/Components/IEntityBehaviorTreeComponent.h>

class CAssistantComponent final : public IEntityComponent
{
public:
	CAssistantComponent() = default;
	virtual ~CAssistantComponent() = default;

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
	}

	//Send BevaviorTree SIGNALS
	void SendEventBehaviorTree(string signalName);

	bool MoveToPlayer();
	bool FollowToPlayer();

	bool Searching(Vec3 searchPos);

	void SendDRSSignal(string signalName);

	float GetDistanceToPlayer();

private:
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
	Cry::DefaultComponents::CStaticMeshComponent* m_pStaticMesh = nullptr;
	IEntityNavigationComponent* m_pNavigationAgent = nullptr;
	IEntityBehaviorTreeComponent* m_pBehaviorTreeComponent = nullptr;

private:
	IEntity* m_playerEntity;

	//DRS
	DRS::IResponseActor* p_DRSActor = nullptr;

	Vec3 m_currentVelocity = ZERO;

	//Searching
	float m_searchNewPosDuration = 2.0f;
};