#include "network.h"

using namespace httplib;

namespace server
{
    
}

namespace client
{
	// Modify from https://github.com/CNA-Bld/EXNOA-CarrotJuicer
	typedef pair<httplib::Client*, string> clientInfo;
	vector<clientInfo> clientList;

	// cout cannot ensure thread safety
	// Use printf() if the submitted function contains printing.
	void initNotifier()
	{
		auto& c = config::get();
		if (!c.enableNotifier) return;

		auto serverList = c.notifierHost;
		for (int i = 0; i < serverList.size(); i++)
		{
			threadPool.submit([serverList, i, c]
			{
				try
				{
					httplib::Client* cli = nullptr;
					cli = new httplib::Client(serverList[i]);
					cli->set_connection_timeout(0, c.notifierConnectionTimeout);
					clientList.push_back(make_pair(cli, serverList[i]));
					printf("Notifier: Connect to %s successfully\n", serverList[i].c_str());
				}
				catch (exception e)
				{
					printf("Error: %s\n", e.what());
					printf("Plugin will ignore this listening server: %s\n", serverList[i].c_str());
				}
			});
		}
		
	}

	void notifyResponse(std::string& data)
	{
		for (int i = 0; i < clientList.size(); i++)
		{
			threadPool.submit([i, data]
			{
				httplib::Client* cli = clientList[i].first;
				if (auto res = cli->Post("/notify/response", data, "application/x-msgpack"))
				{
					if (res->status != 200)
					{
						printf("Unexpected response code from %s: %d\n", clientList[i].second.c_str(), res->status);
					}
				}
				else
				{
					stringstream ss;
					ss << res.error();
					printf("Failed to notify %s: %s\n", clientList[i].second.c_str(), ss.str().c_str());
				}
			});
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