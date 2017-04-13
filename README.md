# D.I.V.A.H. (DLL Injected VEH API Hook)
This code will create a DLL Injection Launcher which then performs WINAPI Hooking through the use of VEH.

### Launcher.exe:
* Usage: Launcher.exe [target_process.exe]
* Launches a program of your choosing in a suspended state.
* Injects the 'injected_VEH.dll' into the target process.


### injected_VEH.dll:
* WIll unprotect kernel32.dll and place an INT3 (debugger interrupt) on LoadLibraryA.
* Registers a program-wide Vectored Exception Handler and parses exceptions for the LoadLibraryA address.
* When LoadLibraryA is called by the target process, program execution is transferred to our VEH. 
* Hardware breakpoints (4 max) can be implemented at a time to pass execution back to our VEH w/o modifying the original program.
