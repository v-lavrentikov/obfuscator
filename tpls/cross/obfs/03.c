asm volatile(
    ".intel_syntax noprefix\n"
    "test rsp, rsp\n"
    "jnz 1f\n"
    {{ops}}
    "1:\n"
    "push {{reg:1}}\n"
    "lea {{reg:1}}, [rip+2f]\n"
    "push {{reg:1}}\n"
    "ret\n"
    {{ops}}
    "2:\n"
    "pop {{reg:1}}\n"
    ".att_syntax prefix\n"
    ::
    : "memory"
);