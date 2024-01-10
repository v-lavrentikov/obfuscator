{{obf:*}} HINSTANCE hInst = {{api-n:LoadLibraryA}}("{{value:guid}}.dll");
{{obf:*}}

if (hInst) {
    {{obf:*}} {{api-n:ExitProcess}}(0);
    {{obf:*}}
}

{{obf:*}}