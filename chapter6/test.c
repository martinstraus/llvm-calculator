#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "ast.h"
#include "generator.h"

extern int yylineno;
SymbolsTable* symbols;

Node* createNode(NodeType type) {
    Node* n = malloc(sizeof(Node));
    n->type = type;
    return n;
}

int main(int args, char** argv) {
    symbols = createSymbolsTable();

    Node* program = createProgram(
        createNodeList(
            createFunctionDefinition(
                "f", 
                createNodeList(
                    createParameterNode("x")
                ),
                createExprNode(AO_ADD, createReferenceNode("x"), createIntNode(1))
            )
        ),
        NULL,
        createExprNode(AO_ADD, createIntNode(1), createIntNode(4))
    );
       
    generate(program, "test", "t");
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "[line %d]: %s\n", yylineno, s);
    exit(2); // Terminate the program on error
}