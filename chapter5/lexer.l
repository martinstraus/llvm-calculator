%{
#include "parser.tab.h"
%}

%option yylineno

%%

"//"(.)*[\n]    // Comments; dicard everything until newline.
[0-9]+          { yylval.number = atoi(yytext); return NUMBER; }
"+"             { return ADD; }
"-"             { return SUB; }
"*"             { return MUL; }
"/"             { return DIV; }
"("             { return LPAREN; }
")"             { return RPAREN; }
\n              { return EOL; }
[ \t]           { /* Ignore whitespace and tabs */ }
.               { fprintf(stderr, "Unexpected character: %s\n", yytext); }

%%

int yywrap() {
    return feof(yyin);
}
