{{obf:*}} size_t size = (1024 * 1024 * 200) + (1024 * {{value:byte}}) + {{value:byte}}; // ~200 MB
{{obf:*}} void *buf = malloc(size);
{{obf:*}}

if (!buf) {
    {{obf:*}} {{caller-exit}}(0);
    {{obf:*}}
}

{{obf:*}} memset(buf, 0x00, size);
{{obf:*}} free(buf);
{{obf:*}}