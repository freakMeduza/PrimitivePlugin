#pragma once


#include "../Plugin/Plugin.h"

namespace test {

	class TestPlugin : public IPlugin {
	public:
		// Inherited via IPlugin
		virtual void OnCreate() override;
		virtual void OnStart() override;
		virtual void OnStop() override;
		virtual void OnDestroy() override;

	};

}

