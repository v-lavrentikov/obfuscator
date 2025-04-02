#define _obf_API_CALL_0(caller, name)       ((_obf_api_proc_0_t)_obf_api_call((caller).kernel, (caller).get_proc, name))
#define _obf_API_CALL_N(caller, name)       ((_obf_api_proc_n_t)_obf_api_call((caller).kernel, (caller).get_proc, name))

#define _obf_STRING_ALLOC(str, name)        char *name = _obf_alloc_decrypted_string(str, _obf_api_key, _obf_api_key_size);
#define _obf_STRING_REALLOC(str, name)      name = _obf_alloc_decrypted_string(str, _obf_api_key, _obf_api_key_size);
#define _obf_STRING_FREE(str)               _obf_free_decrypted_string(str)

typedef PVOID (WINAPI *_obf_api_proc_0_t)();
typedef PVOID (WINAPI *_obf_api_proc_n_t)(PVOID, ...);

typedef FARPROC (WINAPI *_obf_get_proc_t)(HMODULE, LPCSTR);

typedef struct {
    HMODULE kernel;
    _obf_get_proc_t get_proc;
} _obf_api_caller_t;

static forceinline char *_obf_alloc_decrypted_string(const char *cypher, char *key, int size) {
    int i = 0;

    {{obf:*}}
    char *plain = malloc(size);

    {{obf:*}}
    while (1) {
        // Swap (mirroring 8 bytes) and xor decryption until NULL-terminated byte is found
        {{obf:*}} if (!(plain[i+0] = key[i+0] ^ cypher[i+7])) { {{obf:*}} break; }
        {{obf:*}} if (!(plain[i+1] = key[i+1] ^ cypher[i+6])) { {{obf:*}} break; }
        {{obf:*}} if (!(plain[i+2] = key[i+2] ^ cypher[i+5])) { {{obf:*}} break; }
        {{obf:*}} if (!(plain[i+3] = key[i+3] ^ cypher[i+4])) { {{obf:*}} break; }
        {{obf:*}} if (!(plain[i+4] = key[i+4] ^ cypher[i+3])) { {{obf:*}} break; }
        {{obf:*}} if (!(plain[i+5] = key[i+5] ^ cypher[i+2])) { {{obf:*}} break; }
        {{obf:*}} if (!(plain[i+6] = key[i+6] ^ cypher[i+1])) { {{obf:*}} break; }
        {{obf:*}} if (!(plain[i+7] = key[i+7] ^ cypher[i+0])) { {{obf:*}} break; }
        {{obf:*}} i += 8;
        {{obf:*}}
    }

    {{obf:*}}
    return plain;
}

static forceinline void _obf_free_decrypted_string(char *ptr) {
    int i = 0;

    {{obf:*}}
    while (ptr[i] != 0) {
        {{obf:*}}
        ptr[i++] = 0;
        {{obf:*}}
    }

    {{obf:*}}
    free(ptr);
    {{obf:*}}
}

static forceinline void _obf_get_caller(_obf_api_caller_t *caller) {
    {{obf:*}} char *lib = _obf_alloc_decrypted_string(_obf_api_kernel, _obf_api_key, _obf_api_key_size);
    {{obf:*}} char *api = _obf_alloc_decrypted_string(_obf_api_get_proc_address, _obf_api_key, _obf_api_key_size);
    {{obf:*}} caller->kernel = GetModuleHandle(lib);
    {{obf:*}} caller->get_proc = (_obf_get_proc_t)GetProcAddress(caller->kernel, api);
    {{obf:*}} _obf_free_decrypted_string(api);
    {{obf:*}} _obf_free_decrypted_string(lib);
    {{obf:*}}
}

static forceinline FARPROC _obf_api_call(HMODULE kernel, _obf_get_proc_t get_proc, const char *name) {
    {{obf:*}} char *api = _obf_alloc_decrypted_string(name, _obf_api_key, _obf_api_key_size);
    {{obf:*}} FARPROC proc = get_proc(kernel, api);
    {{obf:*}} _obf_free_decrypted_string(api);
    {{obf:*}} return proc;
}