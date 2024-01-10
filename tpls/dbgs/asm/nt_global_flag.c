asm volatile(
    ".intel_syntax noprefix\n"
    "push {{reg:1}}\n"
    "push {{reg:2}}\n"
    "push 1\n"
    "dec qword ptr [rsp]\n"
    "pop {{reg:1}}\n"
    "mov {{reg:1:b}}, 0x60\n"
    "jz 0f\n"
    {{ops}}
    "1:\n"
    "mov {{reg:1:b}}, byte ptr [{{reg:1}}+0xBC]\n"
    "jz 2f\n"
    {{ops}}
    "0:\n"
    "mov {{reg:1}}, qword ptr gs:[{{reg:1}}]\n"
    "jz 1b\n"
    {{ops}}
    "3:\n"
    "pop {{reg:2}}\n"
    "lea {{reg:1}}, [rip+4f-1]\n"
    "push {{reg:1}}\n"
    "inc qword ptr [rsp]\n"
    "ret\n"
    {{ops}}
    "2:\n"
    "mov {{reg:2:b}}, 0x70\n"
    "and {{reg:1:b}}, {{reg:2:b}}\n"
    "sub {{reg:1:b}}, {{reg:2:b}}\n"
    "jnz 3b\n"
    "div {{reg:1:b}}\n"
    {{ops}}
    "4:\n"
    "pop {{reg:1}}\n"
    ".att_syntax prefix\n"
    ::
    : "memory"
);