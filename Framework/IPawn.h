#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "PlayerInput.h"

struct IPawn : public IEntityComponent, public IInputEvents
{
	IPawn() { }
	~IPawn() { ObserverManager::unsubscribe(this); }

	void HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type = EInputFlagType::Hold)
	{
		switch (type)
		{
		case EInputFlagType::Hold:
		{
			if (activationMode & eIS_Pressed)
			{
				m_inputFlags |= flags;
			}
			else if (activationMode & eIS_Released)
			{
				m_inputFlags &= ~flags;
			}
		}
		break;

		case EInputFlagType::Toggle:
		{
			if (activationMode & eIS_Released)
			{
				// Toggle the bit(s)
				m_inputFlags ^= flags;
			}
		}
		break;
		}
	}

	bool IsMovementRequested() const {
		return m_inputFlags != 0;
	}

protected:
	TInputFlags m_inputFlags{ (TInputFlags)EInputFlag::None };
};