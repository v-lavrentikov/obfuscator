{{obf:*}} HINSTANCE hLib = {{api-n:LoadLibraryA}}("{{value:guid}}.dll");
{{obf:*}}

if (hLib) {
    {{obf:*}} {{caller-exit}}(0);
    {{obf:*}}
}

{{obf:*}}