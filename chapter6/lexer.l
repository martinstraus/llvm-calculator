%{
#include "parser.tab.h"
%}

%option yylineno

%%

"//"(.)*[\n]        // Single-line comments.
^[ \t\r]*\n         { /* Ignore empty lines */ }
"+"                 { return ADD; }
"-"                 { return SUB; }
"*"                 { return MUL; }
"/"                 { return DIV; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
"<-"                { return ASSIGN; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }
"fn"                { return FUNCTION; }
[0-9]+              { yylval.number = atoi(yytext); return NUMBER; }
[a-zA-Z][a-zA-Z_]*  { yylval.text = strdup(yytext); return IDENTIFIER; }
\n                  { return EOL; }
[ \t\r]             { /* Ignore whitespace and tabs */ }
.                   { fprintf(stderr, "Unexpected character: %s\n", yytext); }

%%

int yywrap() {
    return feof(yyin);
}
