#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>

DWORD FindProcessId(const std::wstring& processName) {
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (snapshot == INVALID_HANDLE_VALUE) return 0;

	if (Process32First(snapshot, &processInfo)) {
		do {
			if (!processName.compare(processInfo.szExeFile)) {
				CloseHandle(snapshot);
				return processInfo.th32ProcessID;
			}
		} while (Process32Next(snapshot, &processInfo));
	}

	CloseHandle(snapshot);
	return 0;
}

int wmain(int argc, wchar_t* argv[]) {
	if (argc < 3) {
		std::wcout << L"使い方：	injector.exe [プロセス名] [DLLのフルパス]" << std::endl;
		return 1;
	}

	std::wstring processName = argv[1];
	std::wstring dllPath = argv[2];

	DWORD pid = FindProcessId(processName);
	if (pid == 0) {
		std::wcout << L"Can't find the process：" << processName << std::endl;
		return 1;
	}

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess) {
		std::wcout << L"Can't open the process!" << std::endl;
		return 1;
	}

	LPVOID allocAddr = VirtualAllocEx(hProcess, NULL, dllPath.size() * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(hProcess, allocAddr, dllPath.c_str(), dllPath.size() * sizeof(wchar_t), NULL);

	HMODULE hkernel32 = GetModuleHandle(L"kernel32");
	FARPROC loadLibraryAddr = GetProcAddress(hkernel32, "LoadLibraryW");

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, allocAddr, 0, NULL);

	if (hThread) {
		std::wcout << L"injection Success!" << std::endl;
		CloseHandle(hThread);
	} else {
		std::wcout << L"injection Failed." << std::endl;
	}

	CloseHandle(hProcess);
	return 0;
	
}