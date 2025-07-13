BOOL bResult;

{{obf:*}} HANDLE hProcess = {{api-0:GetCurrentProcess}}();
{{obf:*}}

if ({{api-n:CheckRemoteDebuggerPresent}}(hProcess, &bResult) && bResult) {
    {{obf:*}} {{api-n:ExitProcess}}(0);
    {{obf:*}}
}

{{obf:*}}