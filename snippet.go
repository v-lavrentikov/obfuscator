package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"regexp"
	"strings"
	"unicode"
)

var (
	snippetRegsMap = map[string]map[string]string{
		"rax": {"": "rax", "d": "eax", "w": "ax", "b": "al"},
		"rbx": {"": "rbx", "d": "ebx", "w": "bx", "b": "bl"},
		"rcx": {"": "rcx", "d": "ecx", "w": "cx", "b": "cl"},
		"rdx": {"": "rdx", "d": "edx", "w": "dx", "b": "dl"},
		"rsi": {"": "rsi", "d": "esi", "w": "si", "b": "sil"},
		"rdi": {"": "rdi", "d": "edi", "w": "di", "b": "dil"},
	}
	snippetRegs = []string{"rax", "rbx", "rcx", "rdx", "rsi", "rdi"}
	snippetOps  = []string{
		"mov r1, r2",
		"xchg r1, r2",
		"cmp r1, r2",
		"test r1, r2",
		"add r1, r2",
		"adc r1, r2",
		"sub r1, r2",
		"sbb r1, r2",
		"xor r1, r2",
		"and r1, r2",
		"or r1, r2",
		"jmp r1",
		"call r1",
		"push r1",
		"pop r1",
		"inc r1",
		"dec r1",
		"not r1",
		"neg r1",
		"nop",
		"ret",
		".byte b1",
	}
)

type SnippetType int

const (
	SNPT_TYPE_OBF SnippetType = iota
	SNPT_TYPE_DBG
	SNPT_TYPE_SND
	SNPT_TYPE_VMD
)

func (t SnippetType) String() string {
	switch t {
	case SNPT_TYPE_OBF:
		return "obf"
	case SNPT_TYPE_DBG:
		return "dbg"
	case SNPT_TYPE_SND:
		return "snd"
	case SNPT_TYPE_VMD:
		return "vmd"
	default:
		return "unknown"
	}
}

var snippetTypes = []SnippetType{SNPT_TYPE_OBF, SNPT_TYPE_DBG, SNPT_TYPE_SND, SNPT_TYPE_VMD}

type Variant struct {
	name  string
	code  string
	count int
}

type VariantsMap map[string]*Variant

type Snippet struct {
	template *Template
	typ      SnippetType
	variants VariantsMap
}

func (snippet *Snippet) Key() string {
	return fmt.Sprintf(TPL_SNIPPET_KEY, snippet.template.lang, snippet.template.name)
}

type SnippetsMap map[string]*Snippet

func (snippets SnippetsMap) OneOfVariant(names []string, langMask Lang) (*Variant, *Snippet) {
	usedSnippets := make(SnippetsMap)

	for _, name := range names {
		key := fmt.Sprintf(TPL_SNIPPET_KEY, LANG_ASM, name)
		if snippet, ok := snippets[key]; ok {
			usedSnippets[key] = snippet
		}
		key = fmt.Sprintf(TPL_SNIPPET_KEY, LANG_C, name)
		if snippet, ok := snippets[key]; ok {
			usedSnippets[key] = snippet
		}
	}

	if len(usedSnippets) == 0 {
		return nil, nil
	}

	return usedSnippets.RandomVariant(langMask)
}

func (snippets SnippetsMap) RandomVariant(langMask Lang) (*Variant, *Snippet) {
	// Find the minimum number of used variant
	minCount := math.MaxInt64
	for _, snippet := range snippets {
		if (snippet.template.lang & langMask) == 0 {
			continue
		}

		for _, variant := range snippet.variants {
			if variant.count < minCount {
				minCount = variant.count
			}
		}
	}

	// No snippets found for the selected languages
	if minCount == math.MaxInt64 {
		return nil, nil
	}

	lessUsedVariants := make(map[string][]*Variant) // Less used variants for each snippet
	maxLessUsedVariants := 0

	for _, snippet := range snippets {
		if (snippet.template.lang & langMask) == 0 {
			continue
		}

		var variants []*Variant

		// Get variants used the minimum number of times (less used variants)
		for _, variant := range snippet.variants {
			if variant.count == minCount {
				variants = append(variants, variant)
			}
		}

		if len(variants) > 0 {
			// Find the maximum number of less used variants for snippets
			if len(variants) > maxLessUsedVariants {
				maxLessUsedVariants = len(variants)
			}
			lessUsedVariants[snippet.Key()] = variants
		}
	}

	// Find the snippets with the maxim amount of less used variants
	var lessUsedSnippets []*Snippet
	for name, variants := range lessUsedVariants {
		if len(variants) == maxLessUsedVariants {
			lessUsedSnippets = append(lessUsedSnippets, snippets[name])
		}
	}

	snippet := lessUsedSnippets[randInt(len(lessUsedSnippets))]
	variants := lessUsedVariants[snippet.Key()]
	variant := variants[randInt(len(variants))]

	variant.count++

	return variant, snippet
}

func loadSnippets(templates []*Template, typ SnippetType, numVariants, maxAsmOps int) SnippetsMap {
	snippets := make(SnippetsMap)

	for _, template := range templates {
		snippet := &Snippet{
			template,
			typ,
			make(VariantsMap),
		}

		key := snippet.Key()
		snippets[key] = snippet

		for i := 0; i < numVariants; i++ {
			name := fmt.Sprintf(TPL_VARIANT_NAME, typ, key, i)

			var code string
			if template.lang == LANG_ASM {
				code = fmt.Sprintf(TPL_SNIPPET_ASM_FUNC, name, processAsmCode(template.code, maxAsmOps))
			} else {
				code = fmt.Sprintf(TPL_SNIPPET_C_FUNC, name, processCCode(template.code))
			}

			snippet.variants[name] = &Variant{name, code, 0}
		}
	}

	return snippets
}

func processCCode(code string) string {
	return processCode(code, func(sb *strings.Builder, line string) {
		sb.WriteString(fmt.Sprintf("%4c%s%s", ' ', line, fmt.Sprintln()))
	})
}

func processAsmCode(code string, maxOps int) string {
	return processCode(fillAsmRegs(code), func(sb *strings.Builder, line string) {
		pos := strings.Index(line, "{{ops}}")
		if pos > 0 {
			if maxOps < 1 {
				return
			}
			for i := 0; i <= randInt(maxOps); i++ {
				cmd := snippetOps[randInt(len(snippetOps))]
				cmd = strings.Replace(cmd, "r1", snippetRegs[randInt(len(snippetRegs))], 1)
				cmd = strings.Replace(cmd, "r2", snippetRegs[randInt(len(snippetRegs))], 1)
				cmd = strings.Replace(cmd, "b1", fmt.Sprintf("0x%02X", randInt(256)), 1)
				format := fmt.Sprintf("%%%dc\"%%s\\n\"%%s", pos+4)
				sb.WriteString(fmt.Sprintf(format, ' ', cmd, fmt.Sprintln()))
			}
		} else {
			sb.WriteString(fmt.Sprintf("%4c%s%s", ' ', line, fmt.Sprintln()))
		}
	})
}

func fillAsmRegs(code string) string {
	regsMap := make(map[string]string)
	regsPool := make([]string, len(snippetRegs))
	copy(regsPool, snippetRegs)

	r := regexp.MustCompile(`{{(reg:[0-9]+)(?:|:(b|w|d))}}`)
	return r.ReplaceAllStringFunc(code, func(str string) string {
		groups := r.FindStringSubmatch(str)
		key := groups[1]
		bits := groups[2]
		reg, ok := regsMap[key]
		if !ok {
			if len(regsPool) == 0 {
				log.Fatalf("Too many registers used. Use no more than %d registers\n", len(snippetRegs))
			}
			i := randInt(len(regsPool))
			reg = regsPool[i]
			regsMap[key] = reg
			regsPool = remove(regsPool, i)
		}
		return snippetRegsMap[reg][bits]
	})
}

func processCode(code string, step func(*strings.Builder, string)) string {
	var sb strings.Builder

	sc := bufio.NewScanner(strings.NewReader(code))
	count := 0

	for sc.Scan() {
		line := strings.TrimRightFunc(sc.Text(), unicode.IsSpace)

		// Remove empty lines from the end of the snippet
		if len(line) == 0 {
			count++
			continue
		}
		for range count {
			sb.WriteString(fmt.Sprintln())
		}
		count = 0

		step(&sb, line)
	}

	return sb.String()
}
