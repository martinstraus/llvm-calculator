#include "grog.yy.c"

int main(int argc, char** argv) {
    yyin = fopen(argv[1], "r");
    TokenType t = yylex();
    while (t != 0) {
        switch (t)
        {
        case IDENTIFIER:
            printf("%s\n", yytext);
            break;
        case NUMBER:
            printf("%d\n", yyval);
            break;
        
        default:
            break;
        }
        t = yylex();
    }
    fclose(yyin);
    return 0;
} 