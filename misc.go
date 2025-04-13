package main

import (
	"cmp"
	"crypto/rand"
	"fmt"
	"log"
	"math/big"
	"os"
	"slices"
	"strings"
)

func remove(s []string, i int) []string {
	s[i] = s[len(s)-1]
	return s[:len(s)-1]
}

func randInt(max int) int {
	n, err := rand.Int(rand.Reader, big.NewInt(int64(max)))
	if err != nil {
		log.Fatalln(err)
	}
	return int(n.Int64())
}

func randBytes(count int) []byte {
	bts := make([]byte, count)
	_, err := rand.Read(bts)
	if err != nil {
		log.Fatalln(err)
	}
	return bts
}

func cFuncName(str string) string {
	var sb strings.Builder
	for _, c := range str {
		if (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') {
			sb.WriteRune(c)
		} else {
			sb.WriteRune('_')
		}
	}
	return sb.String()
}

func formatBytesToCStrings(bts []byte) string {
	if len(bts) == 0 {
		return ""
	}

	var sb strings.Builder
	for i, b := range bts {
		mod := i % 16
		if mod == 0 {
			if i > 0 {
				sb.WriteString("\n")
			}
			sb.WriteString(fmt.Sprintf("%4c\"", ' '))
		}
		sb.WriteString(fmt.Sprintf("\\x%02X", b))
		if mod == 15 || i == len(bts)-1 {
			sb.WriteString("\"")
		}
	}
	return sb.String()
}

func loadFile(path string) string {
	bts, err := os.ReadFile(path)
	if err != nil {
		log.Fatal(err)
	}
	return string(bts)
}

func sortedKeys[K cmp.Ordered, V any](m map[K]V) []K {
	keys := make([]K, 0, len(m))
	for k := range m {
		keys = append(keys, k)
	}
	slices.Sort(keys)
	return keys
}
