#include <stdio.h>

#define BUFFER_SIZE 1024
#define false 0
#define true 1

typedef int bool;

// Lexer

enum Token {
    _EOF = 0,
    FUNC = 1,
    IDENTIFIER = 2
};

typedef struct Lexer {
    char buffer[BUFFER_SIZE];
    int position;
} Lexer;

// Parser

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    FILE* file = fopen(argv[1], "r");
    Lexer lexer = {};
    while (fgets(lexer.buffer, sizeof(lexer.buffer), file) != NULL) {
        lexer.position = 0;
        printf("%s/n", &(lexer.buffer[lexer.position]));
    }
    fclose(file);
    return 0;
}