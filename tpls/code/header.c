#include <windows.h>
#include <intrin.h>

#ifdef _MSC_VER
    #define forceinline __forceinline
#elif defined(__GNUC__)
    #define forceinline inline __attribute__((__always_inline__))
#elif defined(__CLANG__)
    #if __has_attribute(__always_inline__)
        #define forceinline inline __attribute__((__always_inline__))
    #else
        #define forceinline inline
    #endif
#else
    #define forceinline inline
#endif

static char *const _obf_data =
{{data}}
;

{{data-defs}}

{{custom-defs}}

{{asm-snippets}}

{{api}}

{{shell}}

{{c-snippets}}