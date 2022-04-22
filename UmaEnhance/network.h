#pragma once
#include "uma.h"
#include "config.h"
#include <httplib.h>

namespace server
{
	
}

namespace client
{
	void initNotifier();

	void notifyResponse(std::string& data);
}

namespace msg
{
	json praseRequestPack(const string& data);
}