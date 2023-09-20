#include <stdio.h>
#include <stdlib.h>
#include "grog.tab.h"

int main() {
    yyparse();
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Error: %s\n", s);
    exit(1); // Terminate the program on error
}