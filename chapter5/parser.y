%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

// These two externs are to avoid warnings while compiling.
extern int yylex (void);
void yyerror(const char* s);

Program* root;
SymbolsTable* symbols;

%}

%union {
    int number;
    char* text;
    struct Node* node;
    struct Program* program;
}

/* Define tokens */
%token <number> NUMBER
%token <text> IDENTIFIER
%token ADD SUB MUL DIV
%token LPAREN RPAREN
%token ASSIGN
%token SEMICOLON EOL

%type <program> calc
%type <node> expr assign ret statements

%left ADD SUB
%left MUL DIV

%%

/* Start symbol */
calc: statements ret end { root = createProgram($1, $2); }
    ;

statements:
    statements assign SEMICOLON { $$ = chainStatements($1, $2); }
    | assign SEMICOLON          { $$ = $1;   }
    |                           { $$ = NULL; }
    ;

assign:
    IDENTIFIER ASSIGN expr { 
        $$ = createAssignNode($1, $3);
        createAndAddSymbol(symbols, $$->name, $$);
    }
    ;

ret:
    expr SEMICOLON { $$ = $1; }
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