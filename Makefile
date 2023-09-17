build: grog lexer

grog: src/parser.c
	mkdir -p build
	gcc -g -o build/grogc src/parser.c

lexer: src/lexer.c
	mkdir -p build
	gcc -g -o build/lexer src/lexer.c

clean:
	rm -Rf build
