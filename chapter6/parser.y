%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

// These two externs are to avoid warnings while compiling.
extern int yylex (void);
void yyerror(const char* s);

Node* root;

%}

%union {
    int number;
    char* text;
    struct Node* node;
    struct NodeList* statements;
}

/* Define tokens */
%token <number> NUMBER
%token <text> IDENTIFIER
%token ADD SUB MUL DIV
%token LPAREN RPAREN
%token ASSIGN
%token SEMICOLON COMMA EOL
%token FUNCTION 

%type <node> calc expr assign ret function parameter
%type <statements> statements functions parameters 

%left ADD SUB
%left MUL DIV

%%

/* Start symbol */
calc: functions statements ret YYEOF { root = createProgram($1, $2, $3); }
    ;

functions:
    functions function EOL  { $$ = appendNode($1, $2); }
    | function EOL          { $$ = createNodeList($1); }
    |                       { $$ = NULL; }
    ;

function:
    FUNCTION IDENTIFIER LPAREN parameters RPAREN ASSIGN expr { $$ = createFunctionDefinition($2, $4, $7); }
    ;

parameters:
    parameters COMMA parameter  { $$ = appendNode($1, $3); }
    | parameter                 { $$ = createNodeList($1); }
    |                           { $$ = NULL; }
    ;

parameter:
    IDENTIFIER                  { $$ = createParameterNode($1); }
    ;

statements:
    statements assign EOL { $$ = appendNode($1, $2); }
    | assign EOL          { $$ = createNodeList($1); }
    |                     { $$ = NULL; }
    ;

assign:
    IDENTIFIER ASSIGN expr { $$ = createAssignNode($1, $3); }
    ;

ret:
    expr EOL { $$ = $1; }
    ;

/* Expressions */
expr: expr ADD expr { $$ = createExprNode(AO_ADD, $1, $3); }
    | expr SUB expr { $$ = createExprNode(AO_SUB, $1, $3); }
    | expr MUL expr { $$ = createExprNode(AO_MUL, $1, $3); }
    | expr DIV expr { $$ = createExprNode(AO_DIV, $1, $3); }
    | LPAREN expr RPAREN { $$ = $2; }
    | NUMBER { $$ = createIntNode($1); }
    | IDENTIFIER { $$ = createReferenceNode($1); }
    ;

%%