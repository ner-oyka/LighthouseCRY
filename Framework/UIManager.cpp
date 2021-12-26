//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"
#include "UIManager.h"

CUIManager::CUIManager()
{
	GameEvents::CPawnEvents::Get()->Subscribe(this);

	if (gEnv->pFlashUI)
	{
		m_pTargetElement = gEnv->pFlashUI->GetUIElement("target");
		m_pTargetElement->SetVisible(false);
	}
}

CUIManager::~CUIManager()
{
	GameEvents::CPawnEvents::Get()->Unsubscribe(this);
}

void CUIManager::OnStartSelectTargetForAssistant()
{
	if (m_pTargetElement)
	{
		m_pTargetElement->SetVisible(true);
	}
}

void CUIManager::OnReleaseSelectTargetForAssistant()
{
	if (m_pTargetElement)
	{
		m_pTargetElement->SetVisible(false);
	}
}

void CUIManager::OnPawnStartFight()
{
	if (m_pTargetElement)
	{
		m_pTargetElement->SetVisible(true);
	}
}

void CUIManager::OnPawnReleaseFight()
{
	if (m_pTargetElement)
	{
		m_pTargetElement->SetVisible(false);
	}
}
