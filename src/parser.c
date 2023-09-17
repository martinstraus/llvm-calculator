#include <stdio.h>

#define BUFFER_SIZE 1024
#define false 0
#define true 1
#define END_OF_STRING '\0'
#define LEXER_NOT_INITIALIZED -1
#define NEWLINE '\n'

typedef int bool;

// Lexer

enum TokenType {
    _EOF = 0,
    FUNC = 1,
    IDENTIFIER = 2
};

typedef struct Token {
    enum TokenType type;
    char* textValue;
} Token;

typedef struct Lexer {
    bool initialized;
    FILE* file;
    char buffer[BUFFER_SIZE];
    int position;
    int rollback;
} Lexer;

char* readFileIntoBuffer(Lexer *lexer) {
    return fgets(lexer->buffer, sizeof(lexer->buffer), lexer->file);
}

char nextchar(Lexer *lexer) {
    lexer->position++;
    if (!lexer->initialized || lexer->position >= BUFFER_SIZE || lexer->buffer[lexer->position] == END_OF_STRING) {
        lexer->initialized = true; // redundant after the 1st pass, I know...
        if (readFileIntoBuffer(lexer) == NULL) {
            return EOF;
        }
        lexer->position = 0;
    }
    if (lexer->buffer[lexer->position] == END_OF_STRING) {
        return EOF;
    }
    return lexer->buffer[lexer->position];
}

void rollback(Lexer *lexer) {
    lexer->position = lexer->rollback;
}

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(char c) {
    return c > '0' && c < '9';
}

Token identifier(Lexer* lexer) {
    Token t = {};
    char c = nextchar(lexer);
    while (isDigit(c)) {

    }
    return t;
}

Token nextToken(Lexer *lexer) {
    lexer->rollback = lexer->position;
    char c = nextchar(lexer);
    if (isLetter(c)) {
        rollback(lexer);
        return identifier(lexer);
    }
}

// Parser

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    Lexer lexer = {};
    lexer.initialized = false;
    lexer.file = fopen(argv[1], "r");
    char current_char = nextchar(&lexer);
    while (current_char != EOF) {
        printf("%c", current_char);
        current_char = nextchar(&lexer);
    }
    fclose(lexer.file);
    return 0;
}