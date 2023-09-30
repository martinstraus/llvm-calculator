%{
#include "parser.tab.h"
%}

%%

[0-9]+      { yylval = atoi(yytext); return NUMBER; }
"+"         { return ADD; }
"-"         { return SUB; }
"*"         { return MUL; }
"/"         { return DIV; }
\n          { return EOL; }
[ \t]       { /* Ignore whitespace and tabs */ }

.           { fprintf(stderr, "Unexpected character: %s\n", yytext); }

%%

int yywrap() {
    return feof(yyin);
}
