%{
#include <stdio.h>
#include "ast.h"

Node* root;

%}

%union {
    int number;
    char* text;
    struct Node* node;
}

/* Define tokens */
%token <number> NUMBER
%token <text> IDENTIFIER
%token ADD SUB MUL DIV
%token LPAREN RPAREN
%token ASSIGN
%token SEMICOLON EOL

%type <node> calc expr assign

%%

/* Start symbol */
calc: assign expr end { root = $1; }
    ;

assign:
    IDENTIFIER ASSIGN expr SEMICOLON { $$ = createAssignNode($1, $3); }
    ;

end:
    YYEOF
    ;

/* Expressions */
expr: expr ADD expr { $$ = createExprNode(NT_ADD, $1, $3); }
    | expr SUB expr { $$ = createExprNode(NT_SUB, $1, $3); }
    | expr MUL expr { $$ = createExprNode(NT_MUL, $1, $3); }
    | expr DIV expr { $$ = createExprNode(NT_DIV, $1, $3); }
    | LPAREN expr RPAREN { $$ = $2; }
    | NUMBER { $$ = createIntNode($1); }
    | IDENTIFIER { $$ = createReferenceNode($1); }
    ;

%%