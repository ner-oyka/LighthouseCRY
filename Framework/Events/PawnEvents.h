//Deer Solar Games | 2021 | Lighthouse project

#pragma once

#include <CryEntitySystem/IEntitySystem.h>
#include "External/ObserverManager/ObserverManager.h"

namespace GameEvents
{
	enum class EPawnEvent
	{
		Assistant_StartSelectTarget = BIT(0),
		Assistant_SetTarget = BIT(1),
		Assistant_ReleaseSelectTarget = BIT(2),
		Pawn_StartFight = BIT(3),
		Pawn_ReleaseFight = BIT(4)
	};

	struct IPawnEvents : public BaseObserverProtocol
	{
		/// <summary>
		/// ����� ������ ���� ��� ����������
		/// </summary>
		virtual void OnStartSelectTargetForAssistant() {}
		/// <summary>
		/// ��������� ���� ��� ����������
		/// </summary>
		virtual void OnSetTargetForAssistant() {}
		/// <summary>
		/// ����� �� ������ ������ ���� ��� ����������
		/// </summary>
		virtual void OnReleaseSelectTargetForAssistant() {}
		/// <summary>
		/// ���� � ����� ���
		/// </summary>
		virtual void OnPawnStartFight() {}
		/// <summary>
		/// ����� �� ������ ���
		/// </summary>
		virtual void OnPawnReleaseFight() {}
	};

	class CPawnEvents final
	{
	public:
		CPawnEvents() = default;
		~CPawnEvents() {};

		static CPawnEvents* Get()
		{
			static CPawnEvents instance;
			return &instance;
		}

		void SendEvent(EPawnEvent pawnEvent);

		void Subscribe(IPawnEvents* entity);
		void Unsubscribe(IPawnEvents* entity);
	};
}