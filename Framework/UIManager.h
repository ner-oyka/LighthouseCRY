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
	virtual void OnStartSelectTargetForAssistant();
	virtual void OnReleaseSelectTargetForAssistant();

private:
	IUIElement* m_pTargetElement = nullptr;
};