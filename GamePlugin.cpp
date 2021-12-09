#include "StdAfx.h"
#include "GamePlugin.h"

#include "Framework/PlayerController.h"

#include <IGameObjectSystem.h>
#include <IGameObject.h>

#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>
#include <CrySchematyc/Utils/SharedString.h>

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

#include <CrySystem/ILocalizationManager.h>

#include "DRS/ActionShowGameSubtitle.h"

CGamePlugin::~CGamePlugin()
{
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

	if (gEnv->pSchematyc)
	{
		gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(CGamePlugin::GetCID());
	}
}

bool CGamePlugin::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	// Register for engine system events, in our case we need ESYSTEM_EVENT_GAME_POST_INIT to load the map
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CGamePlugin");


	//Localization setup
	ILocalizationManager* pLocMan = GetISystem()->GetLocalizationManager();
	string sLocaFolderName = "Libs/Localization/";
	string sFileName;
	sFileName.Format("localization.xml");

	string locaFile = sLocaFolderName + sFileName.c_str();

	if (pLocMan->InitLocalizationData(locaFile.c_str()))
	{
		if (!gEnv->IsEditor())
		{
			pLocMan->LoadLocalizationDataByTag("init");
		}
	}

	return true;
}

void CGamePlugin::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
		// Called when the game framework has initialized and we are ready for game logic to start
	case ESYSTEM_EVENT_GAME_POST_INIT:
	{
		// Don't need to load the map in editor
		if (!gEnv->IsEditor())
		{
			// Load the example map in client server mode
			gEnv->pConsole->ExecuteString("map dev_b s", false, true);
		}

		//DRS
		REGISTER_DRS_CUSTOM_ACTION(CActionShowGameSubtitle);
	}
	break;

	case ESYSTEM_EVENT_REGISTER_SCHEMATYC_ENV:
	{
		// Register all components that belong to this plug-in
		auto staticAutoRegisterLambda = [](Schematyc::IEnvRegistrar& registrar)
		{
			// Call all static callback registered with the CRY_STATIC_AUTO_REGISTER_WITH_PARAM
			Detail::CStaticAutoRegistrar<Schematyc::IEnvRegistrar&>::InvokeStaticCallbacks(registrar);
		};

		if (gEnv->pSchematyc)
		{
			gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
				stl::make_unique<Schematyc::CEnvPackage>(
					CGamePlugin::GetCID(),
					"EntityComponents",
					"Crytek GmbH",
					"Components",
					staticAutoRegisterLambda
					)
			);
		}
	}
	break;
	}
}

CRYREGISTER_SINGLETON_CLASS(CGamePlugin)