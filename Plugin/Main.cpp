#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Plugin.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <exception>
#include <filesystem>

/** 
 * Array of HINSTANCE handlers of loaded dll
 */
static std::vector<HINSTANCE> modules;
/** 
 * Array of pointers to plugin interfaces
 */
static std::vector<std::unique_ptr<IPlugin>> plugins;

/** 
 * Forward declaration
 */
void Run(const std::filesystem::path& PluginFolderPath);
void Cleanup();
void Help();

/**
 * Main 
 */
int main(int argc, char** argv) 
{
	try {
		if(argc > 1)
		{
			const std::filesystem::path pluginFolderPath = argv[1];
			
			Run(pluginFolderPath);
		}
		else
		{
			Help();

			return EXIT_FAILURE;
		}
	}
	catch(const std::exception& exception)
	{
		Cleanup();
		std::cerr << "[ ERROR ]: " << exception.what() << std::endl;
	}

	return EXIT_SUCCESS;
}

/**
 * Run - run the test example, loading module, getting plugin interface
 */
void Run(const std::filesystem::path& Path)
{
	if (Path.empty())
	{
		return;
	}

	/**
	* Some example code for dll loading
	*/
	for (const auto& file : std::filesystem::directory_iterator(Path))
	{
		if (file.path().extension() != ".dll")
		{
			continue;
		}

		std::cout << "Loading module: " << file.path() << std::endl;

		HINSTANCE module = ::LoadLibrary(file.path().string().c_str());

		if (module == nullptr)
		{
			throw std::runtime_error{ "Loading module" + file.path().string() + " FAILURED!" };
		}

		modules.push_back(module);

		GetPluginFunc entryPoint = reinterpret_cast<GetPluginFunc>(reinterpret_cast<void*>(::GetProcAddress(module, "GetPlugin")));

		if (entryPoint == nullptr)
		{
			throw std::runtime_error{ "Invalid plugin DLL, entry point function plugin::IPlugin* GetPlugin() is not defined." };
		}

		plugins.push_back(std::unique_ptr<IPlugin>{ entryPoint() });
		plugins.back()->OnCreate();

		for (auto& plugin : plugins)
		{
			plugin->OnStart();
			plugin->OnStop();
		}

		/*
		 * Here is the place for the some eventloop or other shit
		 */

		for (auto& plugin : plugins)
		{
			plugin->OnStop();
		}

		Cleanup();
	}
}

/**
 * Cleanup - release all the resources, destroy all the plugins and unload all the dll modules
 */
void Cleanup()
{
	for (auto& plugin : plugins)
	{
		plugin->OnDestroy();
	}

	plugins.clear();

	for (auto& module : modules)
	{
		::FreeLibrary(module);
	}

	modules.clear();
}

void Help()
{
	std::cerr << "[ ERROR ]: Incorrect number of arguments.\n\nUsage:\n\n"
		"Plugin.exe | [ PluginFolderPath ] - plugin folder path with modules.\n\n"
		"Example:\n\tPlugin.exe C:\\Plugins" << std::endl;
}