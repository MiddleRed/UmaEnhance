#include "network.h"
#include "config.h"

using namespace httplib;

namespace server
{
    
}

namespace msg
{
	// Modify from https://github.com/CNA-Bld/EXNOA-CarrotJuicer
	const vector<int> header_regions = { 4, 56, 72, 88, 104, 120 };
	json praseRequestPack(const string& data)
	{
		try
		{
			const uint32_t offset = *(uint32_t*)data.c_str();
			if (offset != 166)
			{
				cout << "Unknown offset detected: " << offset << "!\n";
			}

			const auto v = string_view(data);

			return json::from_msgpack(v.substr(4 + offset));
		}
		catch ( exception e )
		{
			printf("Error: %s\n",e.what());
			return NULL;
		}
	}
}