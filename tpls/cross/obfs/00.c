asm volatile(
    ".intel_syntax noprefix\n"
    "push {{reg:1}}\n"
    "push 1\n"
    "dec dword ptr [rsp]\n"
    "pop {{reg:1}}\n"
    "jz 1f\n"
    {{ops}}
    "1:\n"
    "pop {{reg:1}}\n"
    ".att_syntax prefix\n"
    ::
    : "memory"
);