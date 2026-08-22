typedef PVOID (WINAPI *_obf_api_proc_0_t)();
typedef PVOID (WINAPI *_obf_api_proc_n_t)(PVOID, ...);

typedef FARPROC (WINAPI *_obf_get_proc_t)(HMODULE, LPCSTR);

typedef struct {
    HMODULE kernel;
    _obf_get_proc_t get_proc;
} _obf_api_caller_t;

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