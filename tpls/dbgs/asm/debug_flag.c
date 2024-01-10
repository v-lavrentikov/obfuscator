asm volatile(
    ".intel_syntax noprefix\n"
    "push {{reg:1}}\n"
    "xor {{reg:1:d}}, {{reg:1:d}}\n"
    "mov {{reg:1:b}}, 0x60\n"
    "jz 0f\n"
    {{ops}}
    "3:\n"
    "lea {{reg:1}}, [rip+4f-1]\n"
    "push {{reg:1}}\n"
    "inc qword ptr [rsp]\n"
    "ret\n"
    {{ops}}
    "0:\n"
    "mov {{reg:1}}, qword ptr gs:[{{reg:1}}]\n"
    "jz 1f\n"
    {{ops}}
    "2:\n"
    "dec {{reg:1:b}}\n"
    "jnz 3b\n"
    "div {{reg:1:b}}\n"
    {{ops}}
    "1:\n"
    "mov {{reg:1:b}}, byte ptr [{{reg:1}}+2]\n"
    "jz 2b\n"
    {{ops}}
    "4:\n"
    "pop {{reg:1}}\n"
    ".att_syntax prefix\n"
    ::
    : "memory"
);