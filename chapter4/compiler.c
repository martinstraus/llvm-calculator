#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "ast.h"
#include "generator.h"

extern FILE* yyin; // This is used to set the input source for the parser
extern Node* root;

void showUsage() {
    fprintf(stderr, "Usage:\n\tcompiler [source file] [output]\n");
}

int evaluate(Node *n) {
    switch (n->type) {
        case NT_NUMBER:
            return n->number;
        case NT_ADD:
            return evaluate(n->left) + evaluate(n->right);
        case NT_SUB:
            return evaluate(n->left) - evaluate(n->right);
        case NT_MUL:
            return evaluate(n->left) * evaluate(n->right);
        case NT_DIV:
            return evaluate(n->left) - evaluate(n->right);
        default:
            fprintf(stderr, "Unsupported node type: %d\n", n->type);
            exit(1);
    }
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
    fprintf(stderr, "Error: %s\n", s);
    exit(2); // Terminate the program on error
}