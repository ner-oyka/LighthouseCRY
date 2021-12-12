#pragma once

#include <array>
#include <numeric>

#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <CryMath/Cry_Camera.h>

#include <ICryMannequin.h>
#include <CrySchematyc/Utils/EnumFlags.h>

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Input/InputComponent.h>
#include <DefaultComponents/Audio/ListenerComponent.h>
#include <DefaultComponents/Physics/RagdollComponent.h>

#include "Framework/IPawn.h"

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CAlexPlayer final : public IPawn
{
public:
	CAlexPlayer() = default;
	virtual ~CAlexPlayer() override final { };

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CAlexPlayer>& desc)
	{
		desc.SetGUID("{63F4C0C6-32AF-4ACB-8FB0-57D45DD14725}"_cry_guid);
		desc.AddMember(&CAlexPlayer::m_isMovement, 'ismv', "IsMovement", "Is Movement", "Alex is movement.", true);
		desc.AddMember(&CAlexPlayer::m_isFight, 'isft', "IsFight", "Is Fight", "Alex join to fight.", false);
		desc.AddMember(&CAlexPlayer::m_isDeath, 'isdt', "IsDeath", "Is Death", "Alex death.", false);
		desc.AddMember(&CAlexPlayer::m_isResearchTargeting, 'rstg', "IsResearchTargeting", "Is Research Targeting", "ResearchTargeting.", false);
	}

	void PhysicalizeDeath();
	void UpdateMovement();
	void UpdateFightMovement();

	Vec3 GetMovement(const Quat& baseRotation);
	Vec3 GetXiMovement(const Quat& baseRotation);
	
	void UpdateAnimation();

	void UpdateLookAt();

	IEntity* FindCover();
	void CheckCover();

	IEntity* GetTargetMeshForAssistant() { return m_pTargetMeshToAssistant; }
	void SpawnMeshTargetForAssistant();

	void SpawnCursorEntity();
	void RemoveCursorEntity();
	void UpdateCursor();
	Vec3 GetCursorWorldPos() { return m_cursorPositionInWorld; }

	//Set states
	void SetMovement(bool val) { m_isMovement = val; }
	void SetFight(bool val) { m_isFight = val; }

private:
	Quat RandomLookAt();

protected:
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CRagdollComponent* m_pRagdollComponent = nullptr;

	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	TagID m_rotateTagId;
	TagID m_crouchTagId;

	Vec2 m_mouseDeltaRotation;

	FragmentID m_activeFragmentId;

	Vec3 m_xiMovementDelta{ ZERO };
	Vec3 m_xiLookDelta{ ZERO };

	//Movement
	float m_speed = 0.0f;
	const float m_baseSpeed = 1.5f;
	const float m_sprintSpeed = 4.5f;
	//smooth speed
	float m_newSpeed;

	Vec3 m_lastMovementRequest = Vec3(0);

	//Cursor
	Vec3 m_cursorPositionInWorld = ZERO;
	IEntity* m_pCursorEntity = nullptr;
	float mouseX, mouseY;

	//Target Mesh For Assistant
	IEntity* m_pTargetMeshToAssistant = nullptr;


	//Collision
	IEntity* m_pCollisionEntity = nullptr;
	Vec3 m_collisionVelocity = Vec3(0);
	Vec3 m_collisionWorldPos = Vec3(0);

	//States
	bool m_isMovement = true;	//Движение
	bool m_isFight = false;		//Бой
	bool m_isSwim = false;		//Плавание
	bool m_isDeath = false;		//Смерть
	bool m_isResearchTargeting = false;	//Режим выбора цели для изучения ассистентом

	//Cover system
	IEntity* currentCoverEntity{ nullptr };

	//LookAt
	bool m_bRandomLook = false;
	float m_globalLookDuration = 4.f;
	float m_localLookDuration = 0.7f;

	float m_currentGlobalLookTime;
	float m_currentLocalLookTime;

	Quat m_currentRandomLookRotation{ IDENTITY };

	IAnimationOperatorQueuePtr m_lookAtModifier;

private:
	//IInputEvents
	virtual void OnForward(int activationMode, float value) override;
	virtual void OnBackward(int activationMode, float value) override;
	virtual void OnLeft(int activationMode, float value) override;
	virtual void OnRight(int activationMode, float value) override;

	virtual void OnShift(int activationMode, float value) override;
	virtual void OnXI_B(int activationMode, float value) override;

	virtual void OnMouseX(int activationMode, float value) override;
	virtual void OnMouseY(int activationMode, float value) override;

	virtual void OnMouseButtonLeft(int activationMode, float value) override;
	virtual void OnMouseButtonRight(int activationMode, float value) override;

	virtual void OnYawDeltaXILeft(int activationMode, float value) override;
	virtual void OnPitchDeltaXILeft(int activationMode, float value) override;

	virtual void OnYawDeltaXIRight(int activationMode, float value) override;
	virtual void OnPitchDeltaXIRight(int activationMode, float value) override;

	virtual void OnEnter(int activationMode, float value) override;
};
