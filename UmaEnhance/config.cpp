#include "config.h"

#define CONFIG_READ_PROPERTY(field) if (j.contains(#field)) config.field = j.at(#field)

namespace config
{
	string configFile = "uconfig.json";
	string _URL_ = "https://api-umamusume.cygames.jp/umamusume";
	string DEF[] = { "MsgPack","asset.txt" };

	config_struct config =
	{
		true,	// enableConsole

		30,		// fps

		false,	// switchServer
		_URL_,	// serverUrl
		7770,	// receivePort
		false,	// showMsgDebug

		true,	// saveRequestPack
		true,	// saveResponsePack
		DEF[0],	// savePackPath

		0,		// printAssetPath;
		DEF[1],	// saveAssetPathFilePath;

		false,	// enableNotifier
		"",		// notifierHost
		100,	// notifierConnectionTimeout

		false	// forceClosingGame
	};

	void raiseError(string e)
	{
		throw e;
		exit(0);
	}

	void loadConfig()
	{

		if (!filesystem::exists(configFile))
		{
			return;
		}
		try
		{
			json j;
			ifstream fs(configFile);
			fs >> j;

			CONFIG_READ_PROPERTY(enableConsole);

			CONFIG_READ_PROPERTY(fps);

			CONFIG_READ_PROPERTY(switchServer);
			CONFIG_READ_PROPERTY(serverUrl);
			CONFIG_READ_PROPERTY(receivePort);
			CONFIG_READ_PROPERTY(showMsgDebug);

			CONFIG_READ_PROPERTY(saveRequestPack);
			CONFIG_READ_PROPERTY(saveResponsePack);

			CONFIG_READ_PROPERTY(printAssetPath);
			CONFIG_READ_PROPERTY(saveAssetPathFilePath);

			CONFIG_READ_PROPERTY(enableNotifier);
			CONFIG_READ_PROPERTY(notifierHost);
			CONFIG_READ_PROPERTY(notifierConnectionTimeout);

			CONFIG_READ_PROPERTY(forceClosingGame);

			if (config.serverUrl.length() > 43 and config.switchServer)	
				raiseError("Server address cannot be larger than 43 characters.");
			if (config.switchServer and not config.enableConsole)
				raiseError("You must enable the console to switch server.");
			if (config.fps < -1)	config.fps = -1;

			printf("Successfully loaded config in `econfig.json`\n");
		}
		catch (exception& e)
		{
			cout << "Exception reading config: " << e.what() << endl;
		}
	}

	config_struct& get()
	{
		return config;
	}
}
