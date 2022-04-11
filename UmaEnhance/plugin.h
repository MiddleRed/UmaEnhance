#pragma once
#include "uma.h"
#include "config.h"
#include "src/thread-pool/include/ThreadPool.h"

namespace plugin 
{
	void refreshConfig();

	extern ThreadPool pool;
	void initThreadPool();

	void HandleGameExit();
}

namespace game
{
	void initDMMToken(json pack);
	void fastReboot();
}