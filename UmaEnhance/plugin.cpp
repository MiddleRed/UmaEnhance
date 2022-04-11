#include "plugin.h"

namespace plugin
{
	void refreshConfig() {
		printf("Note: Not all settings can be updated when the game is still open.\n");
		config::loadConfig();
	}

	ThreadPool pool(2);
	bool isThreadPoolInit = false;
	void initThreadPool()
	{		
		if (not isThreadPoolInit)
		{
			isThreadPoolInit = true;
			pool.init();
		}
	}

	void HandleGameExit()
	{
		pool.shutdown();
		exit(0);
	}
}

namespace game
{
	string DMMViewerID, DMMOnetimeToken;
	bool isDMMTokenLoaded = false;
	void initDMMToken(json pack)
	{
		if (not isDMMTokenLoaded)
		{
			try
			{
				DMMViewerID = pack["dmm_viewer_id"];
				DMMOnetimeToken = pack["dmm_onetime_token"];
				printf("Successfully read DMM Token. Fast Reboot is available.\n");
				isDMMTokenLoaded = true;
			}
			catch (const char* msg)
			{
				cout << msg << endl;
				return;
			}
		}
	}
	void fastReboot()
	{
		if (not isDMMTokenLoaded)
		{
			printf("Error: Cannot reboot, can't read DMM Token. Did you enable `inspectMsgPack`?\n");
			return;
		}
		string rebootCMD = R"(
@echo off
setlocal
taskkill /im "umamusume.exe" >NUL

:waitloop
tasklist | find /i "umamusume.exe" >NUL
if %ERRORLEVEL% == 0 goto waitloop

start "" umamusume.exe /viewer_id=)" + DMMViewerID +
" /onetime_token=" + DMMOnetimeToken + "\ndel reboot.bat";
		ofstream bat("reboot.bat");
		if (bat.is_open())
		{
			bat.write(rebootCMD.c_str(), rebootCMD.size());
			bat.close();

			try 
			{
				plugin::HandleGameExit();
				ShellExecuteA(NULL, "open", "cmd.exe", "/c reboot.bat", NULL, SW_HIDE);
			}
			catch (exception e)
			{
				cout << e.what() << endl;
			}
		}
		else
		{
			printf("Cannot make reboot bat. Please try again.\n");
		}
	}
}