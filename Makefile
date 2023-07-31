build: build/grogc

build/grogc:
	mkdir -p build
	gcc -g -o build/grogc src/*.cpp

clean:
	rm -Rf build
