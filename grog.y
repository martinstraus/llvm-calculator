%{
#include <stdio.h>
#include "grog_ast.h"

int result;

%}

%union {
    int number;
}

/* Define tokens */
%token <number> NUMBER
%token ADD SUB MUL DIV
%token EOL

%type <number> calc expr term factor

%%

/* Start symbol */
calc: expr EOL { result = $1; }
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
