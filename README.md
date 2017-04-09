# D.I.V.A.H. (DLL Injection w/ VEH API Hooking)
This code will create an executable and dll that work in conjunctoin with one another.

### EXECUTABLE:
* Launches a program of your choosing in a suspended state.
* Injects the DLL


### DLL:
* WIll unprotect kernel32.dll and place an INT3 (debugger interrupt) on LoadLibraryA.
* Registers a program-wide Vectored Exception Handler and parses exceptions for the LoadLibraryA address.
* When LoadLibraryA is called by the target process, program execution is transferred to our VEH. 
* Hardware breakpoints (4 max) can be implemented at a time to pass execution back to our VEH w/o modifying the original program.
