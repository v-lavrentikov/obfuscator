/**
 * Checks the following CPUID(1) flags and can be easily extended:
 * 
 * ECX[05] - VMX
 *   Virtual Machine Extensions. A value of 1 indicates that the processor supports this technology
 * ECX[31] - Not Used
 *   Always returns 0 for real CPU. And it looks like it returns 1 for the VM
 * EDX[29] - TM
 *   Thermal Monitor. The processor implements the thermal monitor automatic thermal control circuitry (TCC)
 * 
 * Tested on: VirtualBox
 */
asm volatile(
    ".intel_syntax noprefix\n"

    "jmp 2f\n"
    {{ops}}
    "4:\n"    
    "xor eax, eax\n"
    "inc eax\n"         // eax = 0x01 for TEST instructions
    "jmp 5f\n"
    {{ops}}
    "3:"
    "cpuid\n"           // CPUID(1)
    "jmp 4b\n"
    {{ops}}
    "2:\n"
    "xor eax, eax\n"
    "inc eax\n"         // eax = 0x01 for CPUID call
    "jmp 3b\n"
    {{ops}}
    "5:\n"

    // Check: VMX support
    "shr ecx, 5\n"
    "test ecx, eax\n"
    "jnz 10f\n"
    "jmp 0f\n"
    {{ops}}
    "10:\n"

    // Check: Virtualization flag
    "shr ecx, 26\n"
    "test ecx, eax\n"
    "jz 11f\n"
    "jmp 0f\n"
    {{ops}}
    "11:\n"

    // Check: Thermal monitor support
    "shr edx, 29\n"
    "test edx, eax\n"
    "jnz 12f\n"
    "jmp 0f\n"
    {{ops}}
    "12:\n"

    // Exit: Throw a memory access violation exception
    "jmp 1f\n"
    {{ops}}
    "0:\n"
    "jmp rax\n"
    {{ops}}
    "1:\n"

    ".att_syntax prefix\n"
    ::
    : "rax", "rbx", "rcx", "rdx", "memory"
);