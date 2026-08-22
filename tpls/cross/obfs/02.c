asm volatile(
    ".intel_syntax noprefix\n"
    "push {{reg:1}}\n"
    "xor {{reg:1}}, {{reg:1}}\n"
    "xchg {{reg:1}}, {{reg:2}}\n"
    "inc {{reg:2}}\n"
    "inc {{reg:2}}\n"
    "sub {{reg:2}}, 2\n"
    "jz 1f\n"
    {{ops}}
    "1:\n"
    "mov {{reg:2}}, {{reg:1}}\n"
    "pop {{reg:1}}\n"
    ".att_syntax prefix\n"
    ::
    : "memory"
);