char line[128];

{{obf:*}} {{str-alloc:path:/proc/self/status}}
{{obf:*}} {{str-alloc:flags:r}}

{{obf:*}} FILE *fp = {{api-n:fopen}}(path, flags);

{{obf:*}} {{str-free:path}}
{{obf:*}} {{str-free:flags}}
{{obf:*}}

if (!fp) {
    {{obf:*}} {{caller-exit}}(0);
    {{obf:*}}
}

{{obf:*}} long pid = -1;
{{obf:*}} {{str-alloc:tpl:TracerPid:}}
{{obf:*}}

while ({{api-n:fgets}}(line, sizeof(line), fp)) {
    {{obf:*}}

    if ({{api-n:strncmp}}(line, tpl, 10) == 0) {
        {{obf:*}} pid = (long){{api-n:atoi}}(&line[10]);
        {{obf:*}} break;
    }

    {{obf:*}}
}

{{obf:*}} {{str-free:tpl}}
{{obf:*}} {{api-n:memset}}(line, 0x00, sizeof(line));
{{obf:*}} {{api-n:fclose}}(fp);
{{obf:*}}

if (pid != 0) {
    {{obf:*}} {{caller-exit}}(0);
    {{obf:*}}
}

{{obf:*}}