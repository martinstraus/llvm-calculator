%{
#include "parser.tab.h"
%}

%option yylineno

%%

"//"(.)*[\n]        // Single-line comments.
[0-9]+              { yylval.number = atoi(yytext); return NUMBER; }
[a-zA-Z][a-zA-Z_]*  { yylval.text = strdup(yytext); return IDENTIFIER; }
"+"                 { return ADD; }
"-"                 { return SUB; }
"*"                 { return MUL; }
"/"                 { return DIV; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
"<-"                { return ASSIGN; }
";"                 { return SEMICOLON; }
[ \t\n]             { /* Ignore whitespace and tabs */ }
.                   { fprintf(stderr, "Unexpected character: %s\n", yytext); }

%%

int yywrap() {
    return feof(yyin);
}
