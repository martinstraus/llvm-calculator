#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

extern FILE* yyin; // This is used to set the input source for the parser
extern int result;

void showUsage() {
    fprintf(stderr, "Usage:\n\tgrogc [source file]\n");
}

int main(int args, char** argv) {
    if (args < 2) {
        showUsage();
        exit(1);
    }
    yyin = fopen(argv[1], "r");
    yyparse();
    printf("Result: %d\n", result);
    fclose(yyin);
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Error: %s\n", s);
    exit(2); // Terminate the program on error
}