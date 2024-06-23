#include <stdlib.h>
#include "ast.h"

Node* createIntNode(int number) {
    Node* n = malloc(sizeof(Node));
    n->number = number;
    return n;
}

Node* createExprNode(ArithmeticOperator operator, Node* left, Node* right) {
    Node* n = malloc(sizeof(Node));
    n->type = NT_ARITHMETIC_EXPRESSION;
    n->arithmeticExpression = malloc(sizeof(ArithmeticExpression));
    n->arithmeticExpression->operator = operator;
    n->arithmeticExpression->left = left;
    n->arithmeticExpression->right = right;
    return n;
}