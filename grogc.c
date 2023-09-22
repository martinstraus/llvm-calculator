#include <stdio.h>
#include <stdlib.h>
#include "grog.tab.h"

extern FILE* yyin; // This is used to set the input source for the parser

void showUsage() {
    fprintf(stderr, "Usage:\n\tgrogc [source file]\n");
}

int main(int args, char** argv) {
    if (args < 2) {
        showUsage();
        exit(1);
    }
    printf("File: %s\n", argv[1]);
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Error: %s\n", s);
    exit(2); // Terminate the program on error
}