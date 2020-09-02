#include "TestPlugin.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>

namespace test {

	void TestPlugin::OnCreate()
	{
		std::cout << __FUNCTION__ << std::endl;
	}

	void TestPlugin::OnStart()
	{
		std::cout << __FUNCTION__ << std::endl;
	}

	void TestPlugin::OnStop()
	{
		std::cout << __FUNCTION__ << std::endl;
	}

	void TestPlugin::OnDestroy()
	{
		std::cout << __FUNCTION__ << std::endl;
	}

}

extern "C" IPlugin* GetPlugin()
{
	return new test::TestPlugin{};
}