asm volatile(
    ".intel_syntax noprefix\n"
    "push {{reg:1}}\n"
    "mov {{reg:1}}, rsp\n"
    "call 2f\n"
    "jmp 3f\n"
    {{ops}}
    "1:\n"
    "ret\n"
    {{ops}}
    "2:\n"
    "cmp rsp, {{reg:1}}\n"
    "jne 1b\n"
    {{ops}}
    "3:\n"
    "pop {{reg:1}}\n"
    ".att_syntax prefix\n"
    ::
    : "memory"
);