//Deer Solar Games | 2021 | Lighthouse project

#pragma once

#include "Framework/Events/PawnEvents.h"
#include <FlashUI/FlashUI.h>

class CUIManager final : public GameEvents::IPawnEvents
{
public:
	CUIManager();
	~CUIManager();

private:
	virtual void OnStartSelectTargetForAssistant() override;
	virtual void OnReleaseSelectTargetForAssistant() override;

	virtual void OnPawnStartFight() override;
	virtual void OnPawnReleaseFight() override;

private:
	IUIElement* m_pTargetElement = nullptr;
};