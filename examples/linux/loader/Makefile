CC=gcc
DIR=../../..

all: clean loader

install:
	go build -o $(DIR) $(DIR)

loader:
	$(CC) -s $@.c -o $@

clean:
	rm -f loader

source:
	rm -f loader.c
	$(DIR)/obfuscator -dir $(DIR) -tpl loader_tpl.c -os linux -ops 0 -obfs 1 -dbgs 1 -snds 1 -vmds 1 -shell SLgvYmluL3NoAJlQVF9SZmgtY1ReUugHAAAAd2hvYW1pAFZXVF5qO1gPBQ== > loader.c
