{{obf:*}} HANDLE event = {{api-n:CreateEventA}}(NULL, TRUE, FALSE, "{{value:guid}}");

{{obf:*}}
if ((DWORD64){{api-0:GetLastError}}() == ERROR_ALREADY_EXISTS) {
    {{obf:*}} {{api-n:SetEvent}}(event); // Kill parent process
    {{obf:*}}
    return;
}

CHAR path[MAX_PATH];
PROCESS_INFORMATION pi;
STARTUPINFO si;

{{obf:*}} memset(&si, 0x00, sizeof(STARTUPINFO));
{{obf:*}} si.cb = sizeof(STARTUPINFO);

{{obf:*}} HINSTANCE hInst = {{api-n:GetModuleHandleA}}(NULL);
{{obf:*}} {{api-n:GetModuleFileNameA}}(hInst, path, MAX_PATH);
{{obf:*}} {{api-n:CreateProcessA}}(path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
{{obf:*}} {{api-n:WaitForSingleObject}}(event, INFINITE);
{{obf:*}} {{api-n:CloseHandle}}(event);
{{obf:*}} {{api-n:ExitProcess}}(0);
{{obf:*}}