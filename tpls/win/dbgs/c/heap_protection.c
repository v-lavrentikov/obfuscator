PROCESS_HEAP_ENTRY he;

{{obf:*}} memset(&he, 0x00, sizeof(he));
{{obf:*}}

do {
    if (!{{api-n:HeapWalk}}({{api-0:GetProcessHeap}}(), &he)) {
        {{obf:*}}
        return;
    }
    {{obf:*}}
} while (he.wFlags != PROCESS_HEAP_ENTRY_BUSY);

{{obf:*}} DWORD dwResult = *(PDWORD)((PBYTE)he.lpData + he.cbData);
{{obf:*}}

if (dwResult == 0xABABABAB) {
    {{obf:*}} {{api-n:ExitProcess}}(0);
    {{obf:*}}
}

{{obf:*}}