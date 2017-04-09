// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include <Windows.h>

using namespace std;

DWORD ThreadId;
DWORD ThreadHandle;
CONTEXT ThreadContext;
DWORD oldProt;
DWORD LoadAddr = (DWORD)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
LPBYTE pLoadAddr = (LPBYTE)LoadAddr;
char * title1 = "DLL";
char * text = "1. DLL INJECTION\n2. DLL REGISTERS VEH IN TARGET SPACE\n3. TARGET WINAPI HOOKED WITH INT3.\n4. EXECUTION TRANSFERRED TO OUR VEH WHEN WINAPI EXECUTED\n5. PROFIT!\0";
DWORD exceptionContext;

__declspec(naked) void MyHandler() {

	// Restore LoadLibraryA that we int3'd. We're about to HWBP dis bitch
	pLoadAddr[0] = 0x8b;			

	__asm {
		
		mov eax, dword ptr[esp + 0x4]
		mov eax, dword ptr[eax + 0x4]
		mov exceptionContext, eax	// store base address of Exception context

		push ebp
		mov ebp, esp
		pushad


		// check eip for our int3 addr
		mov ebx, eax
		add ebx, 0xb8				// eip
		mov ebx, [ebx]
		cmp ebx, pLoadAddr
		
		// if it's our int3, then set HWBP for ZwVirtualProtect

		// if it's our ZwVirtual, check ebx for "MZP"

			// calculate offset into MZP for x544f4 top of serial

			// set HWBP on x544f4

		// if it's our x544f4
			
			// nop x5456b
		
		// if it's none of our BP's, xor eax, eax to tell system not ours

		push 0
		push 0
		push title
		push text
		push 0
		call MessageBoxA

		jmp $

		// The following code can set HWBP in current thread (max 4 at a given time)
		call GetCurrentThreadId
		mov ThreadId, eax
		push ThreadId
		push 0
		push 0xfff
		call OpenThread
		mov ThreadHandle, eax// store main handle

		sub esp, 0x2d0 // Thread context structure

		push esp
		push ThreadHandle
		call Wow64GetThreadContext

		mov dword ptr ss : [ebp - 0x2D0], 0x10010//; 2d0 - 10010
		mov dword ptr ss : [ebp - 0x2cc], 0x773d230f//; dr0 - code to HWBP on
		mov dword ptr ss : [ebp - 0x2c8], 0//; dr1
		mov dword ptr ss : [ebp - 0x2c4], 0//; dr2
		mov dword ptr ss : [ebp - 0x2c0], 0//; dr3

		mov dword ptr ss : [ebp - 0x2bc], 0//; dr6
		mov dword ptr ss : [ebp - 0x2b8], 0x101//; dr7 - Break on execute

		lea eax, dword ptr ss : [ebp - 0x2D0]
		push eax
		push ThreadHandle
		call Wow64SetThreadContext

		popad
		mov esp, ebp
		pop ebp
		or eax, 0xffffffff
		jmp $
		ret 4
	}
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		// Set int3 on some API that the target will call. This transfers execution to our VEH which is in the
		// context of the target. This allows us to set HWBPs on the Main target thread.

		VirtualProtect(pLoadAddr, 0x5, 0x40, &oldProt);	// UnProtect LoadLibraryA
		pLoadAddr[0] = 0xcc;							// int 3 on loadlibrary

		__asm {
			push MyHandler								// Exception Handler Code
			push 1										// #1 VEH
			mov eax, 0x772d4c41							// RtlAddVectoredExceptionHandler (may need to find addr)
			call eax
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

