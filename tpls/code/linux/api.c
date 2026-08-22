typedef void *(*_obf_api_proc_0_t)();
typedef void *(*_obf_api_proc_n_t)(void *, ...);

typedef void *(*_obf_get_proc_t)(void *restrict, const char *restrict);

typedef struct {
    void *kernel;
    _obf_get_proc_t get_proc;
} _obf_api_caller_t;

static forceinline void _obf_get_caller(_obf_api_caller_t *caller) {
    {{obf:*}} char *lib = _obf_alloc_decrypted_string(_obf_api_kernel, _obf_api_key, _obf_api_key_size);
    {{obf:*}} char *api = _obf_alloc_decrypted_string(_obf_api_get_proc_address, _obf_api_key, _obf_api_key_size);
    {{obf:*}} caller->kernel = dlopen(lib, RTLD_LAZY);
    {{obf:*}} caller->get_proc = (_obf_get_proc_t)dlsym(caller->kernel, api);
    {{obf:*}} _obf_free_decrypted_string(api);
    {{obf:*}} _obf_free_decrypted_string(lib);
    {{obf:*}}
}

static forceinline void *_obf_api_call(void *kernel, _obf_get_proc_t get_proc, const char *name) {
    {{obf:*}} char *api = _obf_alloc_decrypted_string(name, _obf_api_key, _obf_api_key_size);
    {{obf:*}} void *proc = get_proc(kernel, api);
    {{obf:*}} _obf_free_decrypted_string(api);
    {{obf:*}} return proc;
}