// Mod function
#pragma once
#include "uma.h"
#include "config.h"

namespace mod 
{
	void refreshConfig() {
		config::loadConfig();
	}
}