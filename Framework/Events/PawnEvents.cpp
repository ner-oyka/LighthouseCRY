//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"
#include "PawnEvents.h"

namespace GameEvents
{
	void CPawnEvents::SendEvent(EPawnEvent pawnEvent)
	{
		switch (pawnEvent)
		{
		case GameEvents::EPawnEvent::Assistant_StartSelectTarget:
			ObserverManager::notify(&IPawnEvents::OnStartSelectTargetForAssistant);
			break;
		case GameEvents::EPawnEvent::Assistant_SetTarget:
			ObserverManager::notify(&IPawnEvents::OnSetTargetForAssistant);
			break;
		case GameEvents::EPawnEvent::Assistant_ReleaseSelectTarget:
			ObserverManager::notify(&IPawnEvents::OnReleaseSelectTargetForAssistant);
			break;
		default:
			break;
		}
	}

	void CPawnEvents::Subscribe(IPawnEvents* entity)
	{
		ObserverManager::subscribe<IPawnEvents>(entity);
	}

	void CPawnEvents::Unsubscribe(IPawnEvents* entity)
	{
		ObserverManager::unsubscribe(entity);
	}
}