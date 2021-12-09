//Deer Solar Games | 2021 | Lighthouse project

#include "StdAfx.h"

#include <CryGame/IGameFramework.h>
#include <CryFlowGraph/IFlowBaseNode.h>

#include <CryEntitySystem/IEntitySystem.h>

#include <DefaultComponents/Cameras/CameraComponent.h>

class CActivateCameraNode final
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
	CActivateCameraNode(SActivationInfo* pActInfo)
	{
	}

	virtual void GetMemoryUsage(ICrySizer* s) const override
	{
		s->Add(*this);
	}

	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo) override
	{
		return new CActivateCameraNode(pActInfo);
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
		config.nFlags |= EFLN_TARGET_ENTITY;
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
				IEntity* cameraEntity = pActInfo->pEntity;
				if (Cry::DefaultComponents::CCameraComponent* cameraComponent = cameraEntity->GetComponent<Cry::DefaultComponents::CCameraComponent>())
				{
					cameraComponent->Activate();
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

REGISTER_FLOW_NODE("Camera:ActivateCamera", CActivateCameraNode);