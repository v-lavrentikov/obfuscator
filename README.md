# Obfuscator
Binary obfuscation, anti-reversing, anti-debugging and av-bypass framework for Windows
## Description
This framework provides a preprocessor that modifies C source code by inserting ASM and C based code snippets for binary obfuscation, anti-reversing, anti-debugging and av-bypass purposes. It uses predefined templates to create variations of code snippets.

## Templates
The framework provides two types of snippet templates based on the ASM and C codebase. In turn, C templates can include references to ASM templates. Project contains the following template structure:
- obfs
- dbgs
  - asm
  - c
- snds
  - asm
  - c
- vms
  - asm
  - c

ASM template represents the content of the C language `asm volatile()` compiler instruction with assembly code. Its code uses framework-based instructions for obfuscation logic.

C template represents the body of a C language function. Its code uses framework-based instructions for obfuscation logic, Win32 API calls, and more.
## Snippets
The framework contains four types of snippets:
- **Obfuscation snippets (obfs)** are used for binary obfuscation. They contain dummy assembly code that does nothing, and their only purpose is to obfuscate the application's logic. These snippets can only contain ASM code templates. To insert such snippets into C code, use the `{{obf:*}}` instruction
- **Anti-Debugging snippets (dbgs)** are used to determine whether an application is being debugged. These snippets may contain ASC and C code templates. To insert such snippets into C code, use the `{{dbg:*}}` instruction
- **Sandbox Detection snippets (snds)** are used to determine whether the application is running in an antivirus sandbox. These snippets may contain ASC and C code templates. To insert such snippets into C code, use the `{{snd:*}}` instruction
- **Virtual Machine Detection snippets (vms)** are used to detect guest operating systems running under a virtual machine. These snippets may contain ASC and C code templates. To insert such snippets into C code, use the `{{vm:*}}` instruction

In the provided framework-based instructions can be used asterisk `{{obf:*}}` to insert any snippet, snippet name `{{snd:clone}}` or a comma separated list with the snippet names `{{snd:clone,rdtsc}}`. Spaces are not allowed, names may contain letters, numbers and underscores. The snippet name is the same as the template file name without the `*.c` extension. Snippets with different types and codebases can use the same names.
## Code
The framework contains three templates with C code:
- `header.c` template contains all the framework logic that should be inserted into the resulting C file. Use the `{{header}}` instruction to place this code in the C source file
- `api.c` template contains all the logic related to the Win32 API (at the moment only kernel32.dll supported) calls and decryption functions. This code will be placed in the C source file via the header template
- `shell.c` template contains logic related to the shellcode execution. This code will be placed in the C source file via the header template if the shellcode is provided in the command prompt. Shellcode may be called from code using the `{{shell-exec}}` instruction
### Win32 API Calls
The framework provides two types of instructions for API calls (only kernel32.dll supported). Instructions for calls with `{{api-n:...}}(...)` and without `{{api-0:...}}()` arguments. Each instruction should contain the API function name `{{api-n:Sleep}}`, `{{api-0:GetLastError}}`. The function name will be encrypted and added to the application. Before calling a function, its name will be decrypted and filled with zero bytes after use.

To call API functions, the framework uses a special caller structure that should be initialized once in the project's main function and then passed as a parameter to every function that uses API calls or C code snippets. To work with the API caller, use the following instructions:
- `{{caller-init}}` initializes the caller variable
- `{{caller}}` inserts the caller variable, use it to pass caller to the function as a parameter
- `{{caller-ptr}}` inserts the caller pointer, use it to pass caller to the function as a parameter
- `{{caller-var}}` inserts the caller variable declaration, use it to declare a function parameter 
- `{{caller-var-ptr}}` inserts the caller pointer declaration, uses it to declare a function parameter
- `{{caller-cast}}` declares the caller pointer and initializes it by casting from another pointer, use it in case of API callback functions with arguments of type `void*`
### Constants
The framework allows to insert a random constant into the source code using the following instructions:
- `{{value:byte)}}` inserts an 8-bit random integer value
- `{{value:uuid)}}` inserts an UUID as a string of hexadecimal numbers without delimiters
- `{{value:guid)}}` inserts an UUID in the Windows format with braces and delimiters
### Strings
The framework allows to encrypt string variables used in the source code. To declare and encrypt a string, use the following instruction `{{str-alloc:...:...}}`. This instruction allows to declare a string variable using its name and value `{{str-alloc:my_var_name:my variable text}}`. The variable name can contain letters, numbers, and underscores. The characters `{` and `}` in the string text can be escaped with `\` in the cases of `{{`, `}}`, `\{` and `\}`.
### Shellcode
Calling shellcode is an additional feature of this framework. Pass shellcode as a base64 string by using `-shell` command prompt argument. Use the Metasploit tool `msfvenom` with the argument `-f base64` to generate any type of shellcode.
## ASM Snippets
This type of snippets represents the content of the C language `asm volatile()` compiler instruction with assembly code. Use the "memory" clobber to add the memory barrier. The ASM snippet provides two types of framework-based instructions:
- `{{ops}}` inserts dummy assembly instructions that obfuscate the binary. The number of instructions varies from 0 to the value of the `-ops` argument passed on the command prompt. Pass 0 to tell the framework not to generate dummy instructions
- `{{reg:...:...}}` instruction tells the framework to insert a random register to add polymorphism to the fragment. The first argument specifies the register number. The same template will generate different assembly code each time. Up to 6 (numbers from 0 to 9 can be used) general purpose registers can be inserted: `rax`, `rbx`, `rcx`, `rdx`, `rsi`, `rdi`. The second argument tells the framework the size of the register: without argument - the preprocessor inserts a QWORD register, `d` - inserts a DWORD register, `w` - inserts a WORD register, `b` inserts a BYTE register

The framework generates several variants of ASM snippets and inserts them into the resulting source file as inline C functions with names constructed according to the following pattern `<type>_<language>_<name>_<variant number>`.
## C Snippets
This type of snippets represents the body of a C function that takes a caller parameter. The caller is passed to the function by value for better binary obfuscation. The C snippet allows to use any ASM fragments, call API functions and execute shellcode. The framework generates several variants of C snippets and inserts them into the resulting source file as inline C functions with names constructed according to the following pattern `<type>_<language>_<name>_<variant number>`.
## Available Snippets
- obfs
  - **00**, **01**, **02**, **03**, **04**, **05** - six code obfuscation snippets that implement obfuscation techniques such as: Logic Flow Obfuscation, NOP Obfuscation, Anti-Disassembler Code Obfuscation, Trampolines and Instruction Permutations
- dbgs
  - asm
    - **debug_flag** - direct debugger detection by analyzing the PEB structure
    - **heap_flags** - checking the `Flags` and `ForceFlags` heap fields, which are affected by the presence of a debugger
    - **nt_global_flag** - checking of `NtGlobalFlag` by analyzing the PEB structure
  - c
    - **check_remote_debugger_present** - calling `CheckRemoteDebuggerPresent` Win32 API function
    - **is_debugger_present** - calling `IsDebuggerPresent` Win32 API function
- snds
  - c
    - **clone** - creating a copy of the executing process
    - **load_library** - loading a non-existent dynamic library to cheat the sandbox
    - **memory** - allocation of a huge amount (~100 MB) of memory to force the sandbox analysis to end
    - **query_performance_counter** - detecting the difference between the specified and actual process sleep time using the `QueryPerformanceCounter` Win32 API call
    - **rdtsc** - detecting the difference between the specified and actual process sleep time using the `rdtsc` CPU instruction
- vms
  - c
    - **virtual_box** - detecting the presence of the VirtualBox by checking the existence of the pseudo-device `\\.\VBoxMiniRdrDN` on the system
## Execution
The framework provides the following comand prompt arguments:
- `-help` shows framewor command line help
- `-dir` sets the working directory with the tpls folder. May be useful if the binary is called from another location
- `-tpl` sets a C file with a template for processing
- `-ops` sets the maximum number of random operations for the ASM fragment
- `-obfs`, `-dbgs`, `-snds`, `-vms` set the number of variants for each snippet type
- `-shell` sets a Base64 string with shellcode. Use "msfvenom -f base64 ..." to generate
## Example
This project contains an example template with a C program that demonstrates the framework's capabilities. Use the `Makefile` from the `example` directory to create and compile the obfuscated binary. The example uses shellcode to call a Windows calculator that was generated with the Metasploit framework.
## Disclaimer
This project is provided for informational purposes only. The author is not responsible for its malicious use.
## Project Support
You can support this project by donating to the following Ethereum wallet:

ethereum:0x0468DcdE81b69b87ea0A546faA6c5aae2F4FE30b

![ethereum:0x0468DcdE81b69b87ea0A546faA6c5aae2F4FE30b](https://github.com/v-lavrentikov/obfuscator/assets/2562499/bd7220bc-77a7-4182-a763-4e5a3b2ad102)
