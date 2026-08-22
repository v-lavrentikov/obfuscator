#define _obf_API_CALL_0(caller, name)       ((_obf_api_proc_0_t)_obf_api_call((caller).kernel, (caller).get_proc, name))
#define _obf_API_CALL_N(caller, name)       ((_obf_api_proc_n_t)_obf_api_call((caller).kernel, (caller).get_proc, name))

#define _obf_STRING_ALLOC(str, name)        char *name = _obf_alloc_decrypted_string(str, _obf_api_key, _obf_api_key_size);
#define _obf_STRING_REALLOC(str, name)      name = _obf_alloc_decrypted_string(str, _obf_api_key, _obf_api_key_size);
#define _obf_STRING_FREE(str)               _obf_free_decrypted_string(str)

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

{{os-api}}