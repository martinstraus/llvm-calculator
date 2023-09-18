
%{
#include "grog.tab.h"
%}

%%

[a-z][A-Z]+ { yylval = yytext; return ID; }
[0-9]+      { yylval = atoi(yytext); return NUMBER; }
"+"         { return ADD; }
"-"         { return SUB; }
"*"         { return MUL; }
"/"         { return DIV; }
\n          { return EOL; }
