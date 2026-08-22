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
	$(DIR)/obfuscator -dir $(DIR) -tpl loader_tpl.c -os linux -ops 5 -obfs 20 -dbgs 2 -snds 1 -vmds 2 -shell SLgvYmluL3NoAJlQVF9SZmgtY1ReUugHAAAAd2hvYW1pAFZXVF5qO1gPBQ== > loader.c
