#pragma once

#include "includes.h"
#include <Windows.h>

void InitProcess(const char* windowclassName);

void ReadProccesMem(DWORD address, LPVOID buffer, SIZE_T size);
void WriteProccesMem(DWORD address, LPVOID buffer, SIZE_T size);

DWORD GetModuleBaseAddress(DWORD pID, const char* modulename);