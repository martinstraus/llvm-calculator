digit [0-9]
letter [A-Za-z]
ws [ \t\n]

%{
    int count;
    void yyerror(char* );
%}

%%
{ws}                        { /* do nothing */ } // discard whitespace
{letter}({letter}|{digit})* { count++; } // match identifier
.                           { yyerror("unexpected letter\n"); }
%%

int yywrap (){
    return 1;
}

void yyerror(char* error) {
    printf("Error: %s\n", error);
}