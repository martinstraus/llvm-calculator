#include <stdlib.h>
#include "grog_ast.h"

Node* createIntNode(int number) {
    Node* n = malloc(sizeof(Node));
    return n;
}

Node* createExprNode(NodeType type, Node* left, Node* right) {
    Node* n = malloc(sizeof(Node));
    n->type = type;
    n->left = left;
    n->right = right;
    return n;
}