build: parser

parser:
	mkdir -p build
	bison -d -o grog.tab.c grog.y
	flex -o grog.yy.c grog.l
	gcc -g -o grogc grogc.c grog.tab.c grog.yy.c -lfl

clean:
	rm -f ./grogc *.tab.c *.tab.h *.yy.c
