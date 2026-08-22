BOOL bResult;

{{obf:*}} HANDLE hProcess = {{api-0:GetCurrentProcess}}();
{{obf:*}}

if ({{api-n:CheckRemoteDebuggerPresent}}(hProcess, &bResult) && bResult) {
    {{obf:*}} {{caller-exit}}(0);
    {{obf:*}}
}

{{obf:*}}