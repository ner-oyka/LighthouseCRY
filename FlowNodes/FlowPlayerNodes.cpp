//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"

#include <CryGame/IGameFramework.h>
#include <CryFlowGraph/IFlowBaseNode.h>

#include <CryEntitySystem/IEntitySystem.h>
#include "Framework/PlayerController.h"
#include "Pawns/Alex.h"

class CDisablePlayerMovementNode final
	: public CFlowBaseNode<eNCT_Instanced>
{
	enum INPUTS
	{
		eIP_Activate = 0,
	};

	enum OUTPUTS
	{
		eOP_Done = 0
	};

public:
	CDisablePlayerMovementNode(SActivationInfo* pActInfo)
	{
	}

	virtual void GetMemoryUsage(ICrySizer* s) const override
	{
		s->Add(*this);
	}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override
	{
		return new CDisablePlayerMovementNode(pActInfo);
	}

	virtual void GetConfiguration(SFlowNodeConfig& config) override
	{
		static const SInputPortConfig in_config[] = {
			InputPortConfig_Void("Activate", _HELP("")),
			{ 0 }
		};
		static const SOutputPortConfig out_config[] = {
			OutputPortConfig_Void("Done", _HELP("")),
			{ 0 }
		};
		config.pInputPorts = in_config;
		config.pOutputPorts = out_config;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override
	{
		switch (event)
		{
		case eFE_Initialize:
		{
		}
		break;
		case eFE_Activate:
		{
			if (IsPortActive(pActInfo, eIP_Activate))
			{
				if (IPawn* playerPawn = CPlayerController::Get()->GetControlledPawn())
				{
					playerPawn->GetEntity()->GetComponent<CAlexPlayer>()->SetMovement(false);
					ActivateOutput(pActInfo, eOP_Done, true);
				}
				else
				{
					ActivateOutput(pActInfo, eOP_Done, false);
				}
			}
		}
		}
	}
};

class CEnablePlayerMovementNode final
	: public CFlowBaseNode<eNCT_Instanced>
{
	enum INPUTS
	{
		eIP_Activate = 0,
	};

	enum OUTPUTS
	{
		eOP_Done = 0
	};

public:
	CEnablePlayerMovementNode(SActivationInfo* pActInfo)
	{
	}

	virtual void GetMemoryUsage(ICrySizer* s) const override
	{
		s->Add(*this);
	}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override
	{
		return new CEnablePlayerMovementNode(pActInfo);
	}

	virtual void GetConfiguration(SFlowNodeConfig& config) override
	{
		static const SInputPortConfig in_config[] = {
			InputPortConfig_Void("Activate", _HELP("")),
			{ 0 }
		};
		static const SOutputPortConfig out_config[] = {
			OutputPortConfig_Void("Done", _HELP("")),
			{ 0 }
		};
		config.pInputPorts = in_config;
		config.pOutputPorts = out_config;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override
	{
		switch (event)
		{
		case eFE_Initialize:
		{
		}
		break;
		case eFE_Activate:
		{
			if (IsPortActive(pActInfo, eIP_Activate))
			{
				if (IPawn* playerPawn = CPlayerController::Get()->GetControlledPawn())
				{
					playerPawn->GetEntity()->GetComponent<CAlexPlayer>()->SetMovement(true);
					ActivateOutput(pActInfo, eOP_Done, true);
				}
				else
				{
					ActivateOutput(pActInfo, eOP_Done, false);
				}
			}
		}
		}
	}
};

class CFightStartPlayerNode final
	: public CFlowBaseNode<eNCT_Instanced>
{
	enum INPUTS
	{
		eIP_Activate = 0,
	};

	enum OUTPUTS
	{
		eOP_Done = 0
	};

public:
	CFightStartPlayerNode(SActivationInfo* pActInfo)
	{
	}

	virtual void GetMemoryUsage(ICrySizer* s) const override
	{
		s->Add(*this);
	}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override
	{
		return new CFightStartPlayerNode(pActInfo);
	}

	virtual void GetConfiguration(SFlowNodeConfig& config) override
	{
		static const SInputPortConfig in_config[] = {
			InputPortConfig_Void("Activate", _HELP("")),
			{ 0 }
		};
		static const SOutputPortConfig out_config[] = {
			OutputPortConfig_Void("Done", _HELP("")),
			{ 0 }
		};
		config.pInputPorts = in_config;
		config.pOutputPorts = out_config;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override
	{
		switch (event)
		{
		case eFE_Initialize:
		{
		}
		break;
		case eFE_Activate:
		{
			if (IsPortActive(pActInfo, eIP_Activate))
			{
				if (IPawn* playerPawn = CPlayerController::Get()->GetControlledPawn())
				{
					playerPawn->GetEntity()->GetComponent<CAlexPlayer>()->SetFight(true);
					ActivateOutput(pActInfo, eOP_Done, true);
				}
				else
				{
					ActivateOutput(pActInfo, eOP_Done, false);
				}
			}
		}
		}
	}
};

class CFightExitPlayerNode final
	: public CFlowBaseNode<eNCT_Instanced>
{
	enum INPUTS
	{
		eIP_Activate = 0,
	};

	enum OUTPUTS
	{
		eOP_Done = 0
	};

public:
	CFightExitPlayerNode(SActivationInfo* pActInfo)
	{
	}

	virtual void GetMemoryUsage(ICrySizer* s) const override
	{
		s->Add(*this);
	}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override
	{
		return new CFightExitPlayerNode(pActInfo);
	}

	virtual void GetConfiguration(SFlowNodeConfig& config) override
	{
		static const SInputPortConfig in_config[] = {
			InputPortConfig_Void("Activate", _HELP("")),
			{ 0 }
		};
		static const SOutputPortConfig out_config[] = {
			OutputPortConfig_Void("Done", _HELP("")),
			{ 0 }
		};
		config.pInputPorts = in_config;
		config.pOutputPorts = out_config;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override
	{
		switch (event)
		{
		case eFE_Initialize:
		{
		}
		break;
		case eFE_Activate:
		{
			if (IsPortActive(pActInfo, eIP_Activate))
			{
				if (IPawn* playerPawn = CPlayerController::Get()->GetControlledPawn())
				{
					playerPawn->GetEntity()->GetComponent<CAlexPlayer>()->SetFight(false);
					ActivateOutput(pActInfo, eOP_Done, true);
				}
				else
				{
					ActivateOutput(pActInfo, eOP_Done, false);
				}
			}
		}
		}
	}
};

class CGetPlayerNode final
	: public CFlowBaseNode<eNCT_Instanced>
{
	enum INPUTS
	{
		eIP_Activate = 0,
	};

	enum OUTPUTS
	{
		eOP_Done = 0,
		eOP_PlayerId,
	};

public:
	CGetPlayerNode(SActivationInfo* pActInfo)
	{
	}

	virtual void GetMemoryUsage(ICrySizer* s) const override
	{
		s->Add(*this);
	}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override
	{
		return new CGetPlayerNode(pActInfo);
	}

	virtual void GetConfiguration(SFlowNodeConfig& config) override
	{
		static const SInputPortConfig in_config[] = {
			InputPortConfig_Void("Activate", _HELP("")),
			{ 0 }
		};
		static const SOutputPortConfig out_config[] = {
			OutputPortConfig_Void("Done", _HELP("")),
			OutputPortConfig<EntityId>("PlayerId", _HELP("")),
			{ 0 }
		};
		config.pInputPorts = in_config;
		config.pOutputPorts = out_config;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) override
	{
		switch (event)
		{
		case eFE_Initialize:
		{
		}
		break;
		case eFE_Activate:
		{
			if (IsPortActive(pActInfo, eIP_Activate))
			{
				if (IPawn* playerPawn = CPlayerController::Get()->GetControlledPawn())
				{
					EntityId playerId = playerPawn->GetEntityId();
					ActivateOutput(pActInfo, eOP_PlayerId, playerId);
					ActivateOutput(pActInfo, eOP_Done, true);
				}
				else
				{
					ActivateOutput(pActInfo, eOP_Done, false);
				}
			}
		}
		}
	}
};

REGISTER_FLOW_NODE("Player:DisableMovement", CDisablePlayerMovementNode);
REGISTER_FLOW_NODE("Player:EnableMovement", CEnablePlayerMovementNode);
REGISTER_FLOW_NODE("Player:StartFight", CFightStartPlayerNode);
REGISTER_FLOW_NODE("Player:ExitFight", CFightExitPlayerNode);
REGISTER_FLOW_NODE("Player:GetPlayer", CGetPlayerNode);