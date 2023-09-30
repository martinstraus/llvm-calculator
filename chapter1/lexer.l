%{
#include <stdio.h>
%}

%%

[0-9]+      { printf("%s\n", yytext); }
"+"         { printf("add\n"); }
"-"         { printf("sub\n");}
"*"         { printf("mul\n"); }
"/"         { printf("div\n"); }
[ \t\n]       { /* Ignore whitespace, tabs, and newline */ }

.           { fprintf(stderr, "Unexpected character: %s\n", yytext); }

%%

int yywrap() {
    return feof(yyin);
}
