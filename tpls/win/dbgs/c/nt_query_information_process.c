// const DWORD ProcessDebugFlags = 0x1F;
// const DWORD ProcessDebugObjectHandle = 0x1E;

typedef NTSTATUS (NTAPI *NtQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

NTSTATUS status;

{{obf:*}} {{str-alloc:lpName:ntdll.dll}}
{{obf:*}} HMODULE hLib = {{api-n:LoadLibraryA}}(lpName);
{{obf:*}} {{str-free:lpName}}
{{obf:*}}

if (!hLib) {
    {{obf:*}}
    return;
}

{{obf:*}} {{str-realloc:lpName:NtQueryInformationProcess}}
{{obf:*}} auto pfnNtQueryInformationProcess = (NtQueryInformationProcess){{caller-proc}}(hLib, lpName);
{{obf:*}} {{str-free:lpName}}
{{obf:*}}

if (!pfnNtQueryInformationProcess) {
    {{obf:*}}
    return;
}

{{obf:*}} HANDLE hProcess = {{api-0:GetCurrentProcess}}();

DWORD dwDebugPort;
{{obf:*}} status = pfnNtQueryInformationProcess(hProcess, ProcessDebugPort, &dwDebugPort, sizeof(DWORD), NULL);
{{obf:*}}

if (NT_SUCCESS(status) && (dwDebugPort != 0)) {
    {{obf:*}} {{api-n:ExitProcess}}(0);
    {{obf:*}}
}

DWORD dwDebugFlags;
{{obf:*}} status = pfnNtQueryInformationProcess(hProcess, ProcessDebugFlags, &dwDebugFlags, sizeof(DWORD), NULL);
{{obf:*}}

if (NT_SUCCESS(status) && (dwDebugFlags == 0)) {
    {{obf:*}} {{api-n:ExitProcess}}(0);
    {{obf:*}}
}

{{obf:*}} HANDLE hDebugObject = 0;
{{obf:*}} status = pfnNtQueryInformationProcess(hProcess, ProcessDebugObjectHandle, &hDebugObject, sizeof(HANDLE), NULL);
{{obf:*}}

if (NT_SUCCESS(status) && (hDebugObject != 0)) {
    {{obf:*}} {{api-n:ExitProcess}}(0);
    {{obf:*}}
}

{{obf:*}}