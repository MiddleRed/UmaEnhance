#include "config.h"

#define CONFIG_READ_PROPERTY(field) if (j.contains(#field)) config.field = j.at(#field)

namespace config
{
	string configFile = "uconfig.json";
	string _URL_ = "https://api-umamusume.cygames.jp/umamusume";
	string DEF[] = { "MsgPack" };

	config_struct config =
	{
		true,	// enableConsole

		0,		// fps

		false,	// inspectMsgPack
		true,	// saveRequestPack
		true,	// saveResponsePack
		DEF[0],	// savePackPath

		false,	// enableNotifier
		"",		// notifierHost
		3000,	// notifierConnectionTimeout

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

			CONFIG_READ_PROPERTY(inspectMsgPack);
			CONFIG_READ_PROPERTY(saveRequestPack);
			CONFIG_READ_PROPERTY(saveResponsePack);

			CONFIG_READ_PROPERTY(enableNotifier);
			CONFIG_READ_PROPERTY(notifierHost);
			CONFIG_READ_PROPERTY(notifierConnectionTimeout);

			CONFIG_READ_PROPERTY(forceClosingGame);

			if (config.fps < -1)	config.fps = -1;

			printf("Successfully read config in `%s`\n", configFile.c_str());
		}
		catch (exception& e)
		{
			cout << "Exception occurred when reading config: " << e.what() << endl;
		}
	}

	config_struct& get()
	{
		return config;
	}
}
