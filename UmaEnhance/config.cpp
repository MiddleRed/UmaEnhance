#include "config.h"

#define CONFIG_READ_PROPERTY(field) if (j.contains(#field)) config.field = j.at(#field)

namespace config
{
	string configFile = "uconfig.json";
	string _URL_ = "https://api-umamusume.cygames.jp/umamusume";
	string DEF[] = { "MsgPack" };
	vector<string> EMPVS;

	config_struct config =
	{
		true,	// enableConsole

		0,		// fps

		false,	// inspectMsgPack
		true,	// saveRequestPack
		true,	// saveResponsePack
		DEF[0],	// savePackPath

		false,	// enableNotifier
		EMPVS,	// notifierHost
		3000,	// notifierConnectionTimeout

		false,	// forceClosingGame

		EMPVS,	// bootExternalApp
		false	// doExternalAppRelaunch
	};
	config_struct config_default = config;

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
			if (config.fps < -1)	config.fps = -1;

			CONFIG_READ_PROPERTY(inspectMsgPack);
			CONFIG_READ_PROPERTY(saveRequestPack);
			CONFIG_READ_PROPERTY(saveResponsePack);

			CONFIG_READ_PROPERTY(enableNotifier);
			if (j.contains("notifierHost"))
			{
				auto& arr = j.at("notifierHost");
				if (arr.is_array()) {
					for (int i = 0; i < arr.size(); i++)
						config.notifierHost.push_back(string(arr[i]));
				}
				else config.notifierHost.push_back(string(arr));
			}
			CONFIG_READ_PROPERTY(notifierConnectionTimeout);

			CONFIG_READ_PROPERTY(forceClosingGame);

			if (j.contains("bootExternalApp"))
			{
				auto& arr = j.at("bootExternalApp");
				if (arr.is_array()) {
					for (int i = 0; i < arr.size(); i++)
						config.bootExternalApp.push_back(string(arr[i]));
				}
				else config.bootExternalApp.push_back(string(arr));
			}
			CONFIG_READ_PROPERTY(doExternalAppRelaunch);

			printf("Successfully read config in `%s`\n", configFile.c_str());
		}
		catch (exception& e)
		{
			cout << "Exception occurred when reading the config: " << e.what() << endl;
			printf("Plugin will use the default config setting.\n");
			config = config_default;
		}
	}

	config_struct& get()
	{
		return config;
	}
}
