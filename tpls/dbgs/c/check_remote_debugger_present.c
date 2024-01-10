BOOL bDebuggerPresent;

{{obf:*}} HANDLE handle = {{api-0:GetCurrentProcess}}();
{{obf:*}}

if ({{api-n:CheckRemoteDebuggerPresent}}(handle, &bDebuggerPresent) && bDebuggerPresent) {
    {{obf:*}} {{api-n:ExitProcess}}(0);
}

{{obf:*}}