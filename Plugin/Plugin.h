#pragma once

#ifdef _WIN32
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API 
#endif

#include <memory>
#include <string>

class IPlugin
{
public:
	virtual ~IPlugin() = default;

	/**
	 * OnCreate Event: initialization of the plugin
	 *
	 * On this state plugin doesn't have possibility to interact with other plugins.
	 */
	virtual void OnCreate() = 0;

	/**
	 * OnStart Event: start the plugin
	 *
	 * On this state plugin can interact with other plugins.
	 */
	virtual void OnStart() = 0;

	/**
	 * OnStop Event: stop the plugin
	 *
	 * On this state plugin can interact with other plugins.
	 */
	virtual void OnStop() = 0;

	/**
	 * OnDestroy Event: destroy the plugin
	 *
	 * On this state plugin doesn't have possibility to interact with other plugins.
	 */
	virtual void OnDestroy() = 0;
};

PLUGIN_API typedef IPlugin* (__cdecl *GetPluginFunc)();

/**
 * Get the plugin instance
 */
extern "C" PLUGIN_API IPlugin* GetPlugin();

