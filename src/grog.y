%token ID
%token NUMBER
%token ADD SUB MUL DIV
%token EOL

%%

program: expr EOL
    ;

expr:
    expr ADD expr
    | expr SUB expr
    | term
    ;

term:
    term MUL term
    | term DIV term
    | factor
    ;

factor:
    NUMBER
    | ID
    ;
