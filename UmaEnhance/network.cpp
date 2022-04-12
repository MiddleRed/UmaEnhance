#include "network.h"
#include "config.h"

using namespace httplib;

namespace server
{
    
}

namespace client
{
	// From https://github.com/CNA-Bld/EXNOA-CarrotJuicer
	httplib::Client* client = nullptr;

	void initNotifier()
	{
		auto& c = config::get();

		if (!c.enableNotifier) return;

		client = new httplib::Client(c.notifierHost.data());
		client->set_connection_timeout(0, c.notifierConnectionTimeout);
	}

	void notifyResponse(std::string& data)
	{
		if (client == nullptr) return;

		if (auto res = client->Post("/notify/response", data, "application/x-msgpack"))
		{
			if (res->status != 200)
			{
				std::cout << "Unexpected response from listener: " << res->status << std::endl;
			}
		}
		else
		{
			auto err = res.error();
			std::cout << "Failed to notify listener: " << err << std::endl;
		}
	}
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