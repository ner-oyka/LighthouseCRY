//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"
#include "ActionShowGameSubtitle.h"

#include <CryDynamicResponseSystem/IDynamicResponseSystem.h>

#include <CrySystem/ILocalizationManager.h>
#include <FlashUI/FlashUI.h>

DRS::IResponseActionInstanceUniquePtr CActionShowGameSubtitle::Execute(DRS::IResponseInstance* pResponseInstance)
{
	if (gEnv->pFlashUI)
	{
		IUIElement* m_pSubtitleMain= gEnv->pFlashUI->GetUIElement("SubtitlesMain");
		if (m_pSubtitleMain)
		{
			ILocalizationManager* pLocMan = GetISystem()->GetLocalizationManager();
			string outValue;
			if (pLocMan->LocalizeString(m_Text, outValue))
			{
				m_pSubtitleMain->SetVisible(true);

				SUIArguments args;
				args.AddArgument(outValue);
				m_pSubtitleMain->CallFunction("SetText", args);
			}
		}
	}
	return nullptr;
}

string CActionShowGameSubtitle::GetVerboseInfo() const
{
	return string().Format("'%s'", m_Text.c_str());
}

void CActionShowGameSubtitle::Serialize(Serialization::IArchive& ar)
{
	ar(m_Text, "text", "^Text");
}
