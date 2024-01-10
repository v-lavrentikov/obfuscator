asm volatile(
    ".intel_syntax noprefix\n"
    "push {{reg:1}}\n"
    "xchg {{reg:1}}, {{reg:2}}\n"
    "sub {{reg:2}}, {{reg:2}}\n"
    "dec {{reg:2}}\n"
    "add {{reg:2}}, 2\n"
    "sub {{reg:2}}, 3\n"
    "test {{reg:2}}, {{reg:2}}\n"
    "jne 1f\n"
    {{ops}}
    "1:\n"
    "inc {{reg:2}}\n"
    "inc {{reg:2}}\n"
    "cmp {{reg:2}}, {{reg:2}}\n"
    "je 2f\n"
    {{ops}}
    "2:\n"
    "xchg {{reg:2}}, {{reg:1}}\n"
    "pop {{reg:1}}\n"
    ".att_syntax prefix\n"
    ::
    : "memory"
);