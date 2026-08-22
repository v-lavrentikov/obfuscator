asm volatile(
    ".intel_syntax noprefix\n"

    "push {{reg:1}}\n"
    "push {{reg:2}}\n"
    "push {{reg:3}}\n"
    "jmp 1f\n"
    {{ops}}

    // Call sys_exit_group (231)
    "3:\n"
    "xor {{reg:3:d}}, {{reg:3:d}}\n"
    "mov {{reg:3:b}}, 23\n"
    "inc {{reg:3:d}}\n"
    "xchg {{reg:3:d}}, eax\n"
    "not al\n"
    "xor edi, edi\n"
    "syscall\n"
    "jmp 0f\n"
    {{ops}}

    // Result check
    "2:\n"
    "mov {{reg:2}}, rax\n"
    "pop {{reg:3}}\n"
    "xor {{reg:1:d}}, {{reg:1:d}}\n"
    "cmp {{reg:2}}, {{reg:1}}\n"
    "jz 0f\n"
    "jmp 3b\n"
    {{ops}}

    // Call sys_fork (57)
    "1:\n"
    "pop {{reg:2}}\n"
    "xor {{reg:1:d}}, {{reg:1:d}}\n"
    "mov {{reg:1:b}}, 199\n"
    "dec {{reg:1:d}}\n"
    "xchg {{reg:1:d}}, eax\n"
    "not al\n"
    "syscall\n"
    "jmp 2b\n"
    {{ops}}

    // Passed
    "0:\n"
    "pop {{reg:1}}\n"

    ".att_syntax prefix\n"
    ::
    : "memory"
);