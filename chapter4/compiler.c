#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "ast.h"
#include "generator.h"

extern FILE* yyin; // This is used to set the input source for the parser
extern Node* root;

int evaluate(Node *n);

void showUsage() {
    fprintf(stderr, "Usage:\n\tcompiler [source file] [output]\n");
}

int evaluateArithmeticExpression(ArithmeticExpression* expression) {
    switch (expression->operator) {
        case AO_ADD:
            return evaluate(expression->left) + evaluate(expression->right);
        case AO_SUB:
            return evaluate(expression->left) - evaluate(expression->right);
        case AO_MUL:
            return evaluate(expression->left) * evaluate(expression->right);
        case AO_DIV:
            return evaluate(expression->left) - evaluate(expression->right);
    }
}

int evaluate(Node *n) {
    switch (n->type) {
        case NT_NUMBER:
            return n->number;
        case NT_ARITHMETIC_EXPRESSION:
            return evaluateArithmeticExpression(n->arithmeticExpression);
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