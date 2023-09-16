#include <stdio.h>

#define BUFFER_SIZE 1024

// Lexer

enum Token {
    _EOF = 0,
    FUNC = 1,
    IDENTIFIER = 2
};

typedef struct Lexer {
    char buffer[BUFFER_SIZE];
} Lexer;

// Parser

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    FILE* file = fopen(argv[1], "r");
    Lexer lexer = {};
    while (fgets(lexer.buffer, BUFFER_SIZE, file) != NULL) {
        printf("%s", lexer.buffer);
    }
    fclose(file);
    return 0;
}