build: grog lexer

grog: src/parser.c
	mkdir -p build
	gcc -g -o build/grogc src/parser.c

lexer: src/grog.l
	mkdir -p build
	flex -o src/grog.yy.c src/grog.l
	gcc -g -o build/grogl src/lex.c

clean:
	rm -Rf build
