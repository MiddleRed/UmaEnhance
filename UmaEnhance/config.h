#pragma once
#include "uma.h"


namespace config
{
	struct config_struct
	{
		bool enableConsole;

		int fps;

		bool switchServer;
		string serverUrl;
		int receivePort;
		bool showMsgDebug;

		bool saveRequestPack;
		bool saveResponsePack;
		string savePackPath;

		int printAssetPath;
		string saveAssetPathFilePath;

		bool enableNotifier;
		string notifierHost;
		int notifierConnectionTimeout;

		bool forceClosingGame;
	};

	void loadConfig();

	config_struct& get();
}