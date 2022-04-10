#include "uma.h"
#include "utilities.h"
#include "config.h"
#include "network.h"

extern void dump_bytes(void* pos);
extern void createConsole();

#pragma region HOOK_MACRO
#define ADD_HOOK(_name_, _fmt_) \
	auto _name_##_offset = reinterpret_cast<void*>(_name_##_addr); \
	\
	printf(_fmt_, _name_##_offset); \
	dump_bytes(_name_##_offset); \
	\
	MH_CreateHook(_name_##_offset, _name_##_hook, &_name_##_orig); \
	MH_EnableHook(_name_##_offset); 
#pragma endregion
#pragma region SET_FPS
void* set_fps_orig = nullptr;
void set_fps_hook(int value) {
	return reinterpret_cast<decltype(set_fps_hook)*>(set_fps_orig)(config::get().fps);
}
#pragma endregion
#pragma region SERVER_SWITCH
bool isSendRequest = 0;
void* switch_server_orig = nullptr;
Il2CppString* switch_server_hook()
{
	/*
	* Switch server by redefine the server url.
	* Orginal url: https://api-umamusume.cygames.jp/umamusume
	* The maximum length of customize url should be no bigger than 43
	* to prevent the original url wchat_t array out of memory
	*/
	wstring T_url = s2ws(config::get().serverUrl);
	int T_length = T_url.length();

	auto url = reinterpret_cast<decltype(switch_server_hook)*>(switch_server_orig)();

	memset(url->start_char, 0, sizeof(wchar_t) * 43);
	for (int i = 0; i <= T_length; i++)	url->start_char[i] = T_url[i];
	url->length = T_length;

	printf("GameDefine-ServerUrl is set to `");
	wcout << url->start_char << L"`" << endl;
	return url;
}
#pragma endregion

#pragma region REQUEST_MSGPACK
void* request_pack_orig = nullptr;
int request_pack_hook (
	char* src, char* dst, int srcSize, int dstCapacity)
{	
	// Hook LZ4_compress_default_ext
	int ret = reinterpret_cast<decltype(request_pack_hook)*>(request_pack_orig)(
		src, dst, srcSize, dstCapacity);

	auto outPath = (config::get().savePackPath+"\\").append(currentTime()).append("Q.msgpack");
	writeFile(outPath, src, srcSize);
	printf("Wrote request to %s\n", outPath.c_str());

	

	if (config::get().switchServer)	isSendRequest = true;

	return ret;
}
#pragma endregion


#pragma region RESPONSE_MSGPACK
void* response_pack_orig = nullptr;
int response_pack_hook(
	char* src, char* dst, int compressedSize, int dstCapacity)
{
	// Hook LZ4_decompress_safe_ext
	int ret = reinterpret_cast<decltype(response_pack_hook)*>(response_pack_orig)(
		src, dst, compressedSize, dstCapacity);

	string outPath = (config::get().savePackPath+"\\").append(currentTime()).append("R.msgpack");
	writeFile(outPath, dst, ret);
	printf("Wrote response to %s\n", outPath.c_str());


	if (isSendRequest and config::get().switchServer)
	{
		int retry = 5, timeout = 5000;
		for (; retry > 0; retry--)
		{
			if (server::ifReceive())
			{


				json j = json::parse("{\"response_code\": 1, \"data_headers\": {\"viewer_id\": 0, \"sid\": \"\", \"servertime\": 0, \"result_code\": 1, \"notifications\": {\"unread_information_exists\": 1}, \"server_list\": {\"resource_server\": \"prd-storage-umamusume.akamaized.net/dl/\", \"resource_server_cf\": \"prd-storage-umamusume.akamaized.net/dl/\", \"resource_server_login\": \"prd-storage-app-umamusume.akamaized.net/dl/\", \"resource_server_ingame\": \"prd-storage-game-umamusume.akamaized.net/dl/\"}}, \"data\": {\"attest\": 0, \"nonce\": \"\", \"terms_updated\": 0, \"is_tutorial\": 0, \"resource_version\": \"10005320:TcBFfYyi7yUr\"}}");
				vector<uint8_t> new_buffer = json::to_msgpack(j);
				char* new_dst = reinterpret_cast<char*>(&new_buffer[0]);
				memset(dst, 0, dstCapacity);
				memcpy(dst, new_dst, new_buffer.size());
				ret = new_buffer.size();
				break;
			}
			printf("Timeout, try reconnect after %3lfs, retry attempt left: %d\n", timeout / 1000.0, retry);
			Sleep(timeout);
		}
		if (retry <= 0)
			throw "Cannot receive response msgpack.";
	}
	/*
	std::vector<char> buffer(dst, dst + ret);
	json t = json::from_msgpack(buffer);
	cout <<"MODIFY:" << t.dump() << endl;*/
	return ret;
}
#pragma endregion

#pragma region DUMP_ASSET_PATH

#pragma endregion

#pragma region HANDLE_GAME_CLOSE_EVENT
void* force_quit_orig = NULL;
void force_quit_hook(void* _this)
{
	exit(0);
}
#pragma endregion

void initHook()
{
	// Load DLLs
	auto il2cpp_module = GetModuleHandle(L"GameAssembly.dll");
	il2cpp_symbols::init(il2cpp_module);

	auto libnative_module = GetModuleHandle(L"libnative.dll");

	if (true) 
	{
		auto set_fps_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll", "UnityEngine",
			"Application", "set_targetFrameRate", 1
		);
		ADD_HOOK(set_fps, "UnityEngine.Application.set_targetFrameRate at %p \n");
	}

	if (config::get().saveResponsePack) 
	{
		printf("libnative.dll at %p\n", libnative_module);
		auto response_pack_ptr = GetProcAddress(libnative_module, "LZ4_decompress_safe_ext");
		printf("reponse pack at %p\n", response_pack_ptr);
		MH_CreateHook(response_pack_ptr, response_pack_hook, &response_pack_orig);
		MH_EnableHook(response_pack_ptr);
		filesystem::create_directory("MsgPack");
	}

	if (config::get().saveRequestPack)
	{
		auto request_pack_ptr = GetProcAddress(libnative_module, "LZ4_compress_default_ext");
		printf("request pack at %p\n", request_pack_ptr);
		MH_CreateHook(request_pack_ptr, request_pack_hook, &request_pack_orig);
		MH_EnableHook(request_pack_ptr);
		filesystem::create_directory("MsgPack");
	}

	if (config::get().switchServer) 
	{
		thread(server::initClient).detach();
		auto switch_server_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"GameDefine", "get_ApplicationServerUrl", 0
		);
		ADD_HOOK(switch_server, "Gallop.GameDefine.get_ApplicationServerUrl at %p \n");
	}

	if (config::get().forceClosingGame)
	{
		auto force_quit_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"GameSystem", "OnApplicationQuit", 0
		);
		ADD_HOOK(force_quit, "Gallop_GameSystem.onApplicationQuit at %p \n");
	}
	
}


#pragma region HOOK_MAIN

void* load_library_w_orig = nullptr;
HMODULE __stdcall load_library_w_hook(const wchar_t* path)
{
	printf("Saw %ls\n", path);

	// GameAssembly.dll code must be loaded and decrypted while loading criware library
	if (path == L"cri_ware_unity.dll"s)
	{
		initHook();

		MH_DisableHook(LoadLibraryW);
		MH_RemoveHook(LoadLibraryW);

		return LoadLibraryW(path);
	}

	return reinterpret_cast<decltype(LoadLibraryW)*>(load_library_w_orig)(path);
}

void HookAttach()
{
	config::loadConfig();

	if (config::get().enableConsole)	createConsole();
	if (MH_Initialize() != MH_OK)
	{
		printf("Failed to initialize plugins, please restart the game.\n");
		return;
	}
	// system("cls");
	printf("Plugins initialized successfully. Enjoy!\n");

	// print some information there

	MH_CreateHook(LoadLibraryW, load_library_w_hook, &load_library_w_orig);
	MH_EnableHook(LoadLibraryW);
}

void HookDetach()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

#pragma endregion
