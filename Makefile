build: build/grogc

build/grogc:
	mkdir -p build
	gcc -g -o build/grogc src/*.c

clean:
	rm -Rf build
