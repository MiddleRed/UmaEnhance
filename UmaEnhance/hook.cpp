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

#pragma region REQUEST_MSGPACK
void* request_pack_orig = nullptr;
int request_pack_hook (
	char* src, char* dst, int srcSize, int dstCapacity)
{	
	// Hook LZ4_compress_default_ext
	int ret = reinterpret_cast<decltype(request_pack_hook)*>(request_pack_orig)(
		src, dst, srcSize, dstCapacity);

	thread([src, srcSize]() {
		auto outPath = (config::get().savePackPath + "\\").append(currentTime()).append("Q.msgpack");
		writeFile(outPath, src, srcSize);
		printf("Wrote request to %s\n", outPath.c_str());
	}).detach();

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

	thread([dst, ret]() {
		string outPath = (config::get().savePackPath + "\\").append(currentTime()).append("R.msgpack");
		writeFile(outPath, dst, ret);
		printf("Wrote response to %s\n", outPath.c_str());
	}).detach();

	return ret;
}
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

	if (config::get().fps != 0)
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
