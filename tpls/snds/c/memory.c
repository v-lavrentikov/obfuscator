{{obf:*}}
size_t size = (1024 * 1024 * 100) + (1024 * {{value:byte}}) + {{value:byte}}; // ~100 MB
char *buf = (char *)malloc(size);
{{obf:*}}

if (buf) {
    {{obf:*}} memset(buf, 0x00, size);
    {{obf:*}} free(buf);
    {{obf:*}}
} else {
    {{obf:*}} {{api-n:ExitProcess}}(0);
    {{obf:*}}
}

{{obf:*}}