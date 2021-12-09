#pragma once

#include <CryDynamicResponseSystem/IDynamicResponseAction.h>

class CActionShowGameSubtitle final : public DRS::IResponseAction
{
public:
	//////////////////////////////////////////////////////////
		// IResponseAction implementation
	virtual DRS::IResponseActionInstanceUniquePtr Execute(DRS::IResponseInstance* pResponseInstance) override;
	virtual string                                GetVerboseInfo() const override;
	virtual void                                  Serialize(Serialization::IArchive& ar) override;
	virtual const char* GetType() const override { return "Show Game Subtitle"; }
	//////////////////////////////////////////////////////////

private:
	string m_Text;
};
