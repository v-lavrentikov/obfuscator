{{obf:*}} {{str-alloc:name:\\.\VBoxMiniRdrDN}}

if ({{api-n:CreateFileA}}(name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL) != INVALID_HANDLE_VALUE) {
    {{obf:*}} {{str-free:name}}
    {{obf:*}} {{api-n:ExitProcess}}(0);
}

{{obf:*}} {{str-free:name}}