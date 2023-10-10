#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "ast.h"
#include "generator.h"

extern FILE* yyin; // This is used to set the input source for the parser
extern Node* root;
extern int yylineno;

void showUsage() {
    fprintf(stderr, "Usage:\n\tgrogc [source file] [output]\n");
}

int main(int args, char** argv) {
    if (args < 3) {
        showUsage();
        exit(1);
    }
    yyin = fopen(argv[1], "r");
    yyparse();
    generate(root, argv[2]);
    fclose(yyin);
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "[line %d]: %s\n", yylineno, s);
    exit(2); // Terminate the program on error
}