%{
#include <stdio.h>
#include "grog_ast.h"

Node* root;

%}

%union {
    int number;
    struct Node* node;
}

/* Define tokens */
%token <number> NUMBER
%token ADD SUB MUL DIV
%token EOL

%type <node> calc expr term factor

%%

/* Start symbol */
calc: expr EOL { root = $1; }
    ;

/* Expressions */
expr: expr ADD term { $$ = createExprNode(NT_ADD, $1, $3); }
    | expr SUB term { $$ = createExprNode(NT_SUB, $1, $3); }
    | term { $$ = $1; }
    ;

term: term MUL factor { $$ = createExprNode(NT_MUL, $1, $3); }
    | term DIV factor { $$ = createExprNode(NT_DIV, $1, $3); }
    | factor { $$ = $1; }
    ;

factor: NUMBER { $$ = createIntNode($1); }
    ;

%%