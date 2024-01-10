package main

func encryptStrings(defs map[string]*DataDefine) int {
	// Define the maximum string size
	size := 0
	for _, def := range defs {
		if len(def.value) > size {
			size = len(def.value)
		}
	}

	// Use the maximum string size (+ NULL-termination byte) aligned to 8 bytes as key size
	size++
	m := size % 8
	if m > 0 {
		size = size + 8 - m
	}
	key := randBytes(size)

	for _, def := range defs {
		// Align string to 8 bytes
		def.data = append([]byte(def.value), 0)
		m = len(def.data) % 8
		if m > 0 {
			def.data = append(def.data, randBytes(8-m)...)
		}

		// Xor and swap (mirroring 8 bytes) encryption
		for i := 0; i < len(def.data); i += 8 {
			def.data[i+0], def.data[i+7] = def.data[i+7]^key[i+7], def.data[i+0]^key[i+0]
			def.data[i+1], def.data[i+6] = def.data[i+6]^key[i+6], def.data[i+1]^key[i+1]
			def.data[i+2], def.data[i+5] = def.data[i+5]^key[i+5], def.data[i+2]^key[i+2]
			def.data[i+3], def.data[i+4] = def.data[i+4]^key[i+4], def.data[i+3]^key[i+3]
		}
	}

	// Add API key without encryption
	defs[NAME_API_KEY] = &DataDefine{0, "", key}

	return size
}

func encryptShellcode(defs map[string]*DataDefine, shell []byte) int {
	// Align shellcode to 8 bytes
	m := len(shell) % 8
	if m > 0 {
		shell = append(shell, randBytes(8-m)...)
	}
	key := randBytes(len(shell))

	// Xor and swap (mirroring 8 bytes) encryption
	for i := 0; i < len(shell); i += 8 {
		shell[i+0], shell[i+7] = shell[i+7]^key[i+7], shell[i+0]^key[i+0]
		shell[i+1], shell[i+6] = shell[i+6]^key[i+6], shell[i+1]^key[i+1]
		shell[i+2], shell[i+5] = shell[i+5]^key[i+5], shell[i+2]^key[i+2]
		shell[i+3], shell[i+4] = shell[i+4]^key[i+4], shell[i+3]^key[i+3]
	}

	defs[NAME_SHELL] = &DataDefine{0, "", shell}
	defs[NAME_SHELL_KEY] = &DataDefine{0, "", key}

	return len(shell)
}
