#include <stdio.h>
#include "grog.tab.c"

int main(int argc, char** argv) {
    yyparse();
    return 0;
}