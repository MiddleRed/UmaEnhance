#pragma once
#include "uma.h"
#include "config.h"
#include "network.h"

namespace plugin 
{
	void refreshConfig();

	void HandleGameLaunch();
	void HandleGameExit();
}

namespace game
{
	void initDMMToken(json pack);
	void fastReboot();
}