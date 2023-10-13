%{
#include <stdio.h>

// These two externs are to avoid warnings while compiling.
extern int yylex (void);
void yyerror(const char* s);

int result;

%}

/* Define tokens */
%token NUMBER
%token ADD SUB MUL DIV
%token EOL

// This precedence declarations are needed to simplify the grammar rules.
%left ADD SUB 
%left MUL DIV

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