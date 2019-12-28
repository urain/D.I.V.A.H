#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	while (TRUE) {
		HMODULE dll = LoadLibrary(L".\\DLL.dll");
		HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, "hooked");
		HHOOK handle = SetWindowsHookEx(WH_CBT, addr, dll, 0);
		Sleep(2000);
		//UnhookWindowsHookEx(handle);
	}
	return 0;
}
