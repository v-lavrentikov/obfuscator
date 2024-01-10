package main

import (
	"fmt"
	"os"
	"path"
	"path/filepath"
	"strings"
)

const (
	TPL_SNIPPETS_DIR     = "%s/tpls/%ss/%s"
	TPL_SNIPPETS_OBF_DIR = "%s/tpls/obfs"
	TPL_CODE_FILE        = "%s/tpls/code/%s.c"
	TPL_SNIPPET_KEY      = "%s_%s"
	TPL_VARIANT_NAME     = "%s_%s_%02d"
	// TPL_SNIPPET_C_FUNC   = "static void %s(_obf_api_caller_t caller) {\n%s}"
	// TPL_SNIPPET_ASM_FUNC = "static void %s() {\n%s}"
	TPL_SNIPPET_C_FUNC   = "static forceinline void %s(_obf_api_caller_t caller) {\n%s}" // Pass caller by value to force binary obfuscation
	TPL_SNIPPET_ASM_FUNC = "static forceinline void %s() {\n%s}"
	TPL_SNIPPET_C_CALL   = "%s(caller);"
	TPL_SNIPPET_ASM_CALL = "%s();"
	TPL_API_PROC_NAME    = "_obf_api_proc_%s"
	TCP_API_CALL_0       = "_obf_API_CALL_0(caller, %s)"
	TCP_API_CALL_N       = "_obf_API_CALL_N(caller, %s)"
	TPL_DATA_DEFINE      = "#define %-43s (_obf_data + %d)"
	TPL_CUSTOM_DEFINE    = "#define %-43s %v"
	TPL_STRING_NAME      = "_obf_str_%02d_%s"
	TPL_STRING_ALLOC     = "_obf_STRING_ALLOC(%s, %s);"
	TPL_STRING_FREE      = "_obf_STRING_FREE(%s);"

	CODE_SHELL_CALL     = "_obf_execute_shellcode(caller);"
	CODE_CALLER_INIT    = "_obf_api_caller_t caller; _obf_get_caller(&caller);"
	CODE_CALLER         = "caller"
	CODE_CALLER_PTR     = "&caller"
	CODE_CALLER_VAR     = "_obf_api_caller_t caller"
	CODE_CALLER_VAR_PTR = "_obf_api_caller_t *caller"
	CODE_CALLER_CAST    = "_obf_api_caller_t caller = *(_obf_api_caller_t *)"

	NAME_API_KERNEL           = "_obf_api_kernel"
	NAME_API_GET_PROC_ADDRESS = "_obf_api_get_proc_address"
	NAME_API_KEY              = "_obf_api_key"
	NAME_API_KEY_SIZE         = "_obf_api_key_size"
	NAME_SHELL                = "_obf_shellcode"
	NAME_SHELL_KEY            = "_obf_shellcode_key"
	NAME_SHELL_SIZE           = "_obf_shellcode_size"

	valueApiKernel      = "kernel32.dll"
	valueGetProcAddress = "GetProcAddress"
)

type Lang uint

const (
	LANG_C Lang = 1 << iota
	LANG_ASM

	LANG_MASK_ALL = (LANG_C | LANG_ASM)
)

func (lang Lang) String() string {
	switch lang {
	case LANG_C:
		return "c"
	case LANG_ASM:
		return "asm"
	default:
		return "unknown"
	}
}

func (lang Lang) CallFormat() string {
	switch lang {
	case LANG_C:
		return TPL_SNIPPET_C_CALL
	case LANG_ASM:
		return TPL_SNIPPET_ASM_CALL
	default:
		panic("Assert: Invalid Language passed for Snippet Call Format")
	}
}

func (lang Lang) TemplateKey() string {
	switch lang {
	case LANG_C:
		return "{{c-snippets}}"
	case LANG_ASM:
		return "{{asm-snippets}}"
	default:
		panic("Assert: Invalid Language passed for Snippets Template Key")
	}
}

type Template struct {
	lang Lang
	name string
	code string
}

func loadObfTpls() []*Template {
	return loadTemplates(fmt.Sprintf(TPL_SNIPPETS_OBF_DIR, *workingDir), LANG_ASM)
}

func loadTpls(typ SnippetType, lang Lang) []*Template {
	return loadTemplates(fmt.Sprintf(TPL_SNIPPETS_DIR, *workingDir, typ, lang), lang)
}

func loadTemplates(dir string, lang Lang) []*Template {
	var templates []*Template

	files, err := os.ReadDir(dir)
	if err != nil {
		return templates
	}

	for _, file := range files {
		if file.IsDir() {
			continue
		}

		name := file.Name()
		ext := filepath.Ext(name)

		if ext != ".c" {
			continue
		}

		template := &Template{
			lang,
			cFuncName(strings.TrimSuffix(name, ext)),
			loadFile(path.Join(dir, name)),
		}
		templates = append(templates, template)
	}

	return templates
}
