%{
#include <stdio.h>
#include "ast.h"

// These two externs are to avoid warnings while compiling.
extern int yylex (void);
void yyerror(const char* s);

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

%type <node> calc expr

// This precedence declarations are needed to simplify the grammar rules.
%left ADD SUB 
%left MUL DIV

%%

/* Start symbol */
calc: expr EOL { root = $1; }
    ;

/* Expressions */
expr: expr ADD expr { $$ = createExprNode(AO_ADD, $1, $3); }
    | expr SUB expr { $$ = createExprNode(AO_SUB, $1, $3); }
    | expr MUL expr { $$ = createExprNode(AO_MUL, $1, $3); }
    | expr DIV expr { $$ = createExprNode(AO_DIV, $1, $3); }
    | NUMBER { $$ = createIntNode($1); }
    ;

%%