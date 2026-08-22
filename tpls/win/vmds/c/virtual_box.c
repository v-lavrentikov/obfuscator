{{obf:*}} {{str-alloc:lpName:\\.\VBoxMiniRdrDN}}
{{obf:*}} HANDLE hFile = {{api-n:CreateFileA}}(lpName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
{{obf:*}} {{str-free:lpName}}
{{obf:*}}

if (hFile != INVALID_HANDLE_VALUE) {
    {{obf:*}} {{caller-exit}}(0);
    {{obf:*}}
}

{{obf:*}}