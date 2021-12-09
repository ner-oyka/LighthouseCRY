#pragma once

#include <CrySystem/ICryPlugin.h>

// The entry-point of the application
// An instance of CGamePlugin is automatically created when the library is loaded
// We then construct the local player entity and CPlayerComponent instance when OnClientConnectionReceived is first called.
class CGamePlugin
	: public Cry::IEnginePlugin
	, public ISystemEventListener
{
public:
	CRYINTERFACE_SIMPLE(Cry::IEnginePlugin)
		CRYGENERATE_SINGLETONCLASS_GUID(CGamePlugin, "Blank", "f01244b0-a4e7-4dc6-91e1-0ed18906fe7c"_cry_guid)

		PLUGIN_FLOWNODE_REGISTER
		PLUGIN_FLOWNODE_UNREGISTER

		virtual ~CGamePlugin();

	// Cry::IEnginePlugin
	virtual const char* GetCategory() const override { return "Game"; }
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	// ~Cry::IEnginePlugin

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener
};