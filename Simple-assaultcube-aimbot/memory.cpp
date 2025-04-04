#include "includes.h"


DWORD pID = NULL;
HANDLE pHandle;

void InitProcess(const char* windowclassName)
{
	HWND hWnd = FindWindowA(windowclassName, nullptr);
	GetWindowThreadProcessId(hWnd, &pID);
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);
}

void ReadProccesMem(DWORD address, LPVOID buffer, SIZE_T size)
{
	ReadProcessMemory(pHandle, (LPCVOID)address, buffer, size, nullptr);
}

void WriteProccesMem(DWORD address, LPVOID buffer, SIZE_T size)
{
	WriteProcessMemory(pHandle, (LPVOID)address, buffer, size, nullptr);
}

DWORD GetModuleBaseAddress(DWORD pID, const char* modulename)
{
	DWORD baseaddress = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pID);

	if (snapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(moduleEntry);

		if (Module32First(snapshot, &moduleEntry))
		{
			do
			{
				if (!strcmp(moduleEntry.szModule, modulename))
				{
					baseaddress = (DWORD)moduleEntry.modBaseAddr;
				}
			} while (Module32Next(snapshot, &moduleEntry));
		}
		CloseHandle(snapshot);
	}

	return baseaddress;
}