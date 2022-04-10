#include "uma.h"

extern void HookAttach();
extern void HookDetach();

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved
)
{
	WCHAR buffer[MAX_PATH];
	const std::filesystem::path module_path(std::wstring(buffer, GetModuleFileName(nullptr, buffer, MAX_PATH)));

	// check name
	if (module_path.filename() == L"umamusume.exe")
	{
		current_path(module_path.parent_path());

		if (ul_reason_for_call == DLL_PROCESS_ATTACH)
			thread(HookAttach).detach();

		if (ul_reason_for_call == DLL_PROCESS_DETACH)
			HookDetach();
	}
	return TRUE;
}

