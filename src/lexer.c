#include <stdio.h>

#define EOL '\n'
#define EOS '\0'

typedef enum Token {
    _EOF = 0,
    FUNC = 1,
    IDENTIFIER = 2
} Token;

/* This lexer recognizes the following tokens:

NUMBER: [0-9]+
EQ: '=='
NE: '!='
GT: '>'
GE: '>='
LT: '<'
LE: '<='

*/

int lexemeBegin, forward;
char* text;
int textSize;
char current_char;

char nextchar() {
    if (forward >= textSize) {
        // We reached the end of the file.
        return EOF;
    }
    forward++;
    char c = text[forward];
    if (c == EOS) {
        return EOF;
    }
    return c;
}

int main(int argc, char** argv) {
    text = argv[1];
    textSize = sizeof(text);
    forward = -1;
    lexemeBegin = 0;
    char c = nextchar();
    while(c != EOF) {
        printf("%c", c);
        c = nextchar();
    }
    return 0;
}