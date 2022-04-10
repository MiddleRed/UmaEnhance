#include "uma.h"
#include "plugin.h"

void consoleCommand()
{
	string cmd;
	while (1) 
	{
		getline(cin, cmd);

		if (cmd == "/cfg r")	mod::refreshConfig();
		else if (cmd == "/cls")	system("cls");
	}
}

void createConsole()
{
	AllocConsole();
	SetConsoleTitle(L"UmaEnhance");

	// open stdout stream
	FILE* _;
	freopen_s(&_, "CONOUT$", "w", stdout);
	freopen_s(&_, "CONOUT$", "w", stderr);
	freopen_s(&_, "CONIN$", "r", stdin);

	//printf("UmaPlugins   By MidRed\n");
	//printf("Special thanks to CNA-Bld & GEEKiDoS\n");
	//printf("\nInitializing plugins...\n");

	// avoid japanese characters being put abnormally
	SetConsoleOutputCP(CP_UTF8);
	std::locale::global(std::locale(""));

	// print error
	const HANDLE handle = CreateFile(L"CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD mode;
	if (!GetConsoleMode(handle, &mode))
	{
		std::cout << "GetConsoleMode " << GetLastError() << "\n";
	}
	mode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(handle, mode))
	{
		std::cout << "SetConsoleMode " << GetLastError() << "\n";
	}

	thread(consoleCommand).detach();
}




void dump_bytes(void* pos)
{
	printf("Hex dump of %p\n", pos);

	char* memory = reinterpret_cast<char*>(pos);

	for (int i = 0; i < 0x20; i++)
	{
		if (i > 0 && i % 16 == 0)
			printf("\n");

		char byte = *(memory++);

		printf("%02hhX ", byte);
	}

	printf("\n\n");
}