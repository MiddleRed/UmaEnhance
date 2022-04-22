#pragma once
#include "uma.h"

namespace config
{
	struct config_struct
	{
		bool enableConsole;

		int fps;

		bool inspectMsgPack;
		bool saveRequestPack;
		bool saveResponsePack;
		string savePackPath;

		bool enableNotifier;
		string notifierHost;
		int notifierConnectionTimeout;

		bool forceClosingGame;

		vector<string> bootExternalApp;
		bool doExternalAppRelaunch;
	};

	void loadConfig();

	config_struct& get();
}