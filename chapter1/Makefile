CFLAGS = -I/usr/include/llvm-c-14
LDFLAGS = -L/usr/lib/llvm-14
LIBS = -lfl -lLLVM-14

build: parser

parser:
	flex -o lexer.yy.c lexer.l
	gcc -g -o compiler *.c  $(CFLAGS) $(LDFLAGS) $(LIBS)

clean:
	rm -f ./compiler *.tab.c *.tab.h *.yy.c
