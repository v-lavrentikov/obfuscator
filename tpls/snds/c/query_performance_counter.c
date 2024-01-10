LARGE_INTEGER f, t1, t2;
ULONGLONG interval = 3000 + ({{value:byte}} + {{value:byte}}); // ~3.5s

{{obf:*}} {{api-n:QueryPerformanceFrequency}}(&f);
{{obf:*}} {{api-n:QueryPerformanceCounter}}(&t1);
{{obf:*}} {{api-n:Sleep}}((PVOID)interval);
{{obf:*}} {{api-n:QueryPerformanceCounter}}(&t2);

{{obf:*}} double result = (double)(t2.QuadPart - t1.QuadPart) / f.QuadPart * 1000;
{{obf:*}}

if (result < interval) {
    {{obf:*}} {{api-n:ExitProcess}}(0);
    {{obf:*}}
}

{{obf:*}} 