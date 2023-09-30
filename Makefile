CFLAGS = -I/usr/include/llvm-c-14
LDFLAGS = -L/usr/lib/llvm-14
LIBS = -lfl -lLLVM-14

build: parser

parser:
	bison -d -o grog.tab.c grog.y
	flex -o grog.yy.c grog.l
	gcc -g -o grogc *.c  $(CFLAGS) $(LDFLAGS) $(LIBS)

clean:
	rm -f ./grogc *.tab.c *.tab.h *.yy.c
