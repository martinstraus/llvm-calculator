digit [0-9]
letter [A-Za-z]
ws [ \t\n]

%{
typedef enum TokenType {
    IDENTIFIER = 1, NUMBER
} TokenType; 

int yyval, count;
void yyerror(char*);
%}

%%
{ws}                        { /* do nothing */ }                      // discard whitespace
{letter}({letter}|{digit})* { count++; return(IDENTIFIER); }          // match identifier
{digit}+                    { yyval = atoi(yytext); return(NUMBER); } // match number
.                           { yyerror("unexpected letter\n"); }       // reject anything else
%%

int yywrap() {
    return feof(yyin);
}

void yyerror(char* error) {
    printf("Error: %s\n", error);
}
