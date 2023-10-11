#include <stdlib.h>
#include "ast.h"

Node* createIntNode(int number) {
    Node* n = malloc(sizeof(Node));
    n->type = NT_NUMBER;
    n->number = number;
    return n;
}

Node* createReferenceNode(char* name) {
    Node* n = malloc(sizeof(Node));
    n->type = NT_REFERENCE;
    n->name = name;
    return n;
}

Node* createExprNode(NodeType type, Node* left, Node* right) {
    Node* n = malloc(sizeof(Node));
    n->type = type;
    n->left = left;
    n->right = right;
    return n;
}

Node* createAssignNode(char* name, Node* expr){
    Node* n = malloc(sizeof(Node));
    n->type = NT_ASSIGN;
    n->name = name;
    n->expr = expr;
    return n;
}

Program* createProgram(Node* assign, Node* ret) {
    Program* p = malloc(sizeof(Program));
    p->assign = assign;
    p->ret = ret;
    return p;
}

