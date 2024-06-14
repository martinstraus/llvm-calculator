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
    struct FunctionNode* function;
    struct FunctionNode* functions;
    struct ParameterNode* parameter;
    struct ParameterNode* parameters;
}

/* Define tokens */
%token <number> NUMBER
%token <text> IDENTIFIER
%token ADD SUB MUL DIV
%token LPAREN RPAREN
%token ASSIGN
%token SEMICOLON COMMA EOL
%token FUNCTION 

%type <program> calc
%type <node> expr assign ret statements
%type <function> function
%type <functions> functions
%type <parameter> parameter
%type <parameters> parameters

%left ADD SUB
%left MUL DIV

%%

/* Start symbol */
calc: functions statements ret YYEOF { root = createProgram($1, $2, $3); }
    ;

functions:
    functions function EOL  { $$ = addFunctionNode($1, $2); }
    | function EOL          { $$ = $1; }
    |
    ;

function:
    FUNCTION IDENTIFIER LPAREN parameters RPAREN ASSIGN expr { $$ = createFunctionNode($2, $4, $7); }
    ;

parameters:
    parameters COMMA parameter  { $$ = appendParameterNode($1, $3); }
    | parameter                 { $$ = $1; }
    |                           { $$ = NULL; }
    ;

parameter:
    IDENTIFIER                  { $$ = createParameterNode($1); }
    ;

statements:
    statements assign EOL { $$ = chainStatements($1, $2); }
    | assign EOL          { $$ = $1;   }
    |                     { $$ = NULL; }
    ;

assign:
    IDENTIFIER ASSIGN expr { 
        $$ = createAssignNode($1, $3);
        createAndAddSymbol(symbols, $$->name, $$);
    }
    ;

ret:
    expr EOL { $$ = $1; }
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