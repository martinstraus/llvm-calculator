build: grog

grog: src/parser.c
	mkdir -p build
	gcc -g -o build/grogc src/parser.c

clean:
	rm -Rf build
