%{
#include <stdio.h>
%}

/* Define tokens */
%token NUMBER
%token ADD SUB MUL DIV
%token EOL

%%

/* Start symbol */
calc: expr EOL { printf("Result: %d\n", $1); }
    ;

/* Expressions */
expr: expr ADD term { $$ = $1 + $3; }
    | expr SUB term { $$ = $1 - $3; }
    | term { $$ = $1; }
    ;

term: term MUL factor { $$ = $1 * $3; }
    | term DIV factor { $$ = $1 / $3; }
    | factor { $$ = $1; }
    ;

factor: NUMBER { $$ = $1; }
    ;

%%

int main() {
    yyparse();
    return 0;
}

void yyerror(char* s) {
    fprintf(stderr, "Error: %s\n", s);
}