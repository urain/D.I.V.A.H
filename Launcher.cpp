#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <Windows.h>

using namespace std;

int main(int argc, char* argv[]) {

	char * target = argv[1];
	char * dll = ".\\injected_VEH.dll\0";
	char * k32 = "kernel32.dll\0";
	char * loadlibrarya = "LoadLibraryA\0";
	DWORD WINAPI LoadLibraryAddr = 00000000;
	HANDLE ProcHandle = 00000000;
	DWORD ProcId = 00000000;
	LPVOID TargetMem = 00000000;
	HANDLE ThreadHandle = 00000000;
	PROCESS_INFORMATION procinfo;
	STARTUPINFOA startupinfo;
	SIZE_T * temp = 0;

	ZeroMemory(&startupinfo, sizeof(startupinfo));

	try {
    // Get LoadLibraryA virtual addres
		LoadLibraryAddr = (DWORD) GetProcAddress(GetModuleHandleA(k32), loadlibrarya);  
		cout << "LoadLibraryA Address: " << hex << LoadLibraryAddr << endl;
    
		// Start our target process suspended
		CreateProcessA(target, 0, 0, 0, 0, 4, 0, 0, &startupinfo, &procinfo);
		cout << "Created Suspended Process" << endl;

    // Store our target process ID and Thread Handle
		ProcId = procinfo.dwProcessId;
		ThreadHandle = procinfo.hThread;

    // Get an all access handle to our target
		ProcHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcId);
		cout << "Opened Handle" << endl;

    // Allocate some memory in the target
		TargetMem = VirtualAllocEx(ProcHandle, 0, 100, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		cout << "Allocated Memory" << endl;

    // Write our DLL name to our newly allocated memory
		WriteProcessMemory(ProcHandle, TargetMem, dll, strlen(dll), temp);
		cout << "Wrote Memory" << endl;

    // Start a remote thread using LoadLibraryA and our DLL as the argument
		CreateRemoteThread(ProcHandle, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryAddr, TargetMem, 0, 0);
		cout << "Created Remote Thread..." << endl;
		
    // Give time for DLL to set BP and setup Vectored Exception Handler
		Sleep(2000);

    // Resume our target program
		ResumeThread(ThreadHandle);
		cout << "Resumed Thread" << endl;

		Sleep(1000);
	}

	catch(...){
		TerminateProcess(ProcHandle, 0);
	}
	return 0;
}
