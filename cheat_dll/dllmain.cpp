﻿#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		MessageBoxA(NULL, "Cheat DLL injected!", "Cheat", MB_OK | MB_ICONINFORMATION);


	}
	
	return TRUE;
}
