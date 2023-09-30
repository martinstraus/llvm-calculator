%{
#include <stdio.h>

int result;

%}

/* Define tokens */
%token NUMBER
%token ADD SUB MUL DIV
%token EOL

%%

/* Start symbol */
calc: expr EOL { result = $1; }
    ;

/* Expressions */
expr: expr ADD expr { $$ = $1 + $3; }
    | expr SUB expr { $$ = $1 - $3; }
    | expr MUL expr { $$ = $1 * $3; }
    | expr DIV expr { $$ = $1 / $3; }
    | NUMBER { $$ = $1; }
    ;

%%