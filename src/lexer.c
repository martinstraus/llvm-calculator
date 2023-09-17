#include <stdio.h>

#define EOL '\n'
#define EOS '\0'

typedef enum TokenType {
    _EOF = 0,
    BOOL_OP,
    NUMBER
} TokenType;

typedef enum BoolenOperator {
    EQ = 0, NE, GT, GE, LT, LE 
} BoolenOperator;

typedef struct Token {
    TokenType type;
    BoolenOperator booleanOperator;
    int integerNumber;
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

Token BOOL_OP() {
    Token t = {};
    t.type = BOOL_OP;
    int state = 0;
    char c;
    while (1) {
        switch(state) {
            case 0: 
                c = nextchar();
                switch (c) {
                    case '=':
                        state = 1;
                        break;
                    case '!':
                        state= 3;
                        break;
                    case '<':
                        state= 5;
                        break;
                    case '>':
                        state= 8;
                        break;
                    default:
                        break;
                }
                break;
            case 1:
                c = nextchar();
                switch (c) {
                    case '=':
                        state = 2;
                        t.booleanOperator = EQ;
                        return t;
                    default:

                        break;
                }    
                break;
             case 3:
                c = nextchar();
                switch (c) {
                    case '=':
                        state = 2;
                        t.booleanOperator = NE;
                        return t;
                    default:
                        
                        break;
                }    
                break;
            case 5 :
                c = nextchar();
                switch (c) {
                    case '=':
                        state = 2;
                        t.booleanOperator = GE;
                        return t;
                    default:
                        state = 7;
                        t.booleanOperator = GT;
                        return t;
                }    
                break;
            case 8:
                c = nextchar();
                switch (c) {
                    case '=':
                        state = 9;
                        t.booleanOperator = LE;
                        return t;
                    default:
                        state = 10;
                        t.booleanOperator = LT;
                        return t;
                }    
                break;
        }
    }
}

Token NUMBER() {
    Token t = {};
    t.type = NUMBER;
    int state = 0;
    char c;
    while (1) {
        switch(state) {
            case 0:
                c = nextchar();
                if (c >= '0' && c <='9'){
                    state = 1;
                }
                break;
            case 1:
                c = nextchar();
                if (c >= '0' && c <='9'){
                    state = 1;
                }
                break;
            default:
                //t.integerNumber
                return t;
        }
    }
}

Token nextToken() {
    char c = nextchar();

    /*char c = nextchar();
    while(c != EOF) {
        printf("%c", c);
        c = nextchar();
    }*/
}

int main(int argc, char** argv) {
    text = argv[1];
    textSize = sizeof(text);
    forward = -1;
    lexemeBegin = 0;
    Token t = nextToken();
    while (t.type != _EOF) {
        switch (t.type)
        {
        case EQ:
            printf("EQ ");
            break;        
        case NE:
            printf("NE ");
            break;        
        case GT:
            printf("GT ");
            break;        
        case GE:
            printf("GE ");
            break;        
        case LT:
            printf("LT ");
            break;        
        case LE:
            printf("LE ");
            break;        
        default:
            printf("<unexpected token>");
            break;
        }
        t = nextToken();
    }
    printf("EOF\n");
    return 0;
}