#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved) {

	switch (Reason) {
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) int hooked(int code, WPARAM wParam, LPARAM lParam) {
	MessageBoxA(0, (LPCSTR)"hooked", (LPCSTR)"hooked", 0);
	return(CallNextHookEx(NULL, code, wParam, lParam));
}
