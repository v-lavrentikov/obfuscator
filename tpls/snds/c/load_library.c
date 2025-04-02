{{obf:*}} HINSTANCE hLib = {{api-n:LoadLibraryA}}("{{value:guid}}.dll");
{{obf:*}}

if (hLib) {
    {{obf:*}} {{api-n:ExitProcess}}(0);
    {{obf:*}}
}

{{obf:*}}