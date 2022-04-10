#include "network.h"
#include "config.h"

using namespace httplib;

namespace server
{
    string revJ;
	void initClient()
	{
        Server svr;
        printf("Local Client Server initialized.\n");
        svr.Post("/umamusume",
            [&](const Request& req, Response& res, const ContentReader& content_reader) {
                if (req.is_multipart_form_data()) {
                    cout << "handle" << endl;
                    MultipartFormDataItems files;
                    content_reader(
                        [&](const MultipartFormData& file) {
                            files.push_back(file);
                            return true;
                        },
                        [&](const char* data, size_t data_length) {
                            files.back().content.append(data, data_length);
                            return true;
                        });
                }
                else {
                    content_reader([&](const char* data, size_t data_length) {
                        revJ.append(data, data_length);
                        if (config::get().showMsgDebug)
                        {
                            cout<< revJ.length() << endl;
                            printf("%s\n", (revJ.length()>1000 ? revJ.substr(0,1000)+"......" : revJ).c_str());
                        }
                        return true;
                        });
                }
            });

        svr.listen("0.0.0.0", config::get().receivePort);
	}

    bool ifReceive() {
        cout << "RECEIVE " << revJ.length() << endl;
        return revJ.length();
    }

    string getReceive()
    {
        string tmp = "{\"response_code\": 1, \"data_headers\": {\"viewer_id\": 0, \"sid\": \"\", \"servertime\": 0, \"result_code\": 1, \"notifications\": {\"unread_information_exists\": 1}, \"server_list\": {\"resource_server\": \"prd-storage-umamusume.akamaized.net/dl/\", \"resource_server_cf\": \"prd-storage-umamusume.akamaized.net/dl/\", \"resource_server_login\": \"prd-storage-app-umamusume.akamaized.net/dl/\", \"resource_server_ingame\": \"prd-storage-game-umamusume.akamaized.net/dl/\"}}, \"data\": {\"attest\": 0, \"nonce\": \"\", \"terms_updated\": 0, \"is_tutorial\": 0, \"resource_version\": \"10005320:TcBFfYyi7yUr\"}}";
        revJ.clear();
        return tmp;
    }
}

namespace msg
{

}