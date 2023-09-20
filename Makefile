build: parser

parser:
	mkdir -p build
	bison -d -o src/grog.tab.c src/grog.y
	flex -o src/grog.yy.c src/grog.l
	gcc -o build/grogc src/grogc.c src/grog.tab.c src/grog.yy.c -lfl

clean:
	rm -Rf build
	rm src/grog.tab.c src/grog.tab.h src/grog.yy.c src/parser.c
