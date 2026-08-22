asm volatile(
    ".intel_syntax noprefix\n"
    "push {{reg:1}}\n"
    "xor {{reg:1}}, {{reg:1}}\n"
    "sub {{reg:1}}, 2\n"
    "not {{reg:1}}\n"
    "cmp {{reg:1}}, 1\n"
    "je 1f\n"
    {{ops}}
    "1:\n"
    "pop {{reg:1}}\n"
    ".att_syntax prefix\n"
    ::
    : "memory"
);