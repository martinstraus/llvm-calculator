#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#define SYMBOL_ADDED 1
#define SYMBOL_ALREADY_DEFINED 0

/* ABSTRACT SYNTAX TREE */

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

// Quite inneficient implementation, but it works.
Node* chainStatements(Node* first, Node* new) {
    Node* last = first;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = new;
    return first;
}

Program* createProgram(FunctionNode* functions, Node* assign, Node* ret) {
    Program* p = malloc(sizeof(Program));
    p->functions = functions;
    p->assign = assign;
    p->ret = ret;
    return p;
}

/* 
    SYMBOLS TABLE
    All these implementations are incredibly slow, since they traverse the whole linked list.
*/

SymbolsTable* createSymbolsTable() {
    SymbolsTable* st = malloc(sizeof(SymbolsTable));
    st->symbol = NULL;
    st->next = NULL;
    return st; 
}

Symbol* findSymbol(SymbolsTable* table, char* name) {
    SymbolsTable* t = table;
    while (t != NULL && strcmp(name, t->symbol->name) != 0) {
        t = t->next;
    }
    if (t != NULL) {
        return t->symbol;
    } else {
        return NULL;
    }
}

// Returns NULL if the symbol is not in the table.
int containsSymbol(SymbolsTable* table, char* name) {
    return findSymbol(table, name) != NULL;
}

// Beware! it does not check if the symbol is in the table. This could be easily optimized.
int appendSymbol(SymbolsTable* table, Symbol* symbol) {
    SymbolsTable* t = table;
    if (t->symbol != NULL && strcmp(t->symbol->name , symbol->name) == 0) {
        return SYMBOL_ALREADY_DEFINED;
    } 
    while (t->next != NULL) {
        t = t->next;
        if (t->symbol != NULL && strcmp(t->symbol->name , symbol->name) == 0) {
            return SYMBOL_ALREADY_DEFINED;
        }
    }
    t->next = malloc(sizeof(SymbolsTable));
    t->symbol = symbol;
    return SYMBOL_ADDED;
}

void createAndAddSymbol(SymbolsTable* symbols, char* name, Node* value) {
    Symbol* s = malloc(sizeof(Symbol));
    s->name = name;
    s->value = value;
    if (!appendSymbol(symbols, s)) { 
        fprintf(stderr, "Symbol already defined: %s\n", name); exit(1); 
    } 
}

// Functions definitions
ParameterNode* createParameterNode(char* name) {
    ParameterNode* pd = malloc(sizeof(ParameterNode));
    pd->name = name;
    return pd;
}

ParameterNode* appendParameterNode(ParameterNode* first, ParameterNode* next) {
    ParameterNode* l = first;
    while (l->next != NULL) {
        l = l->next;
    }
    l->next = next;
    return l;
}

FunctionNode* createFunctionNode(char* name, ParameterNode* parameters, Node* expression) {
    FunctionNode* fd = malloc(sizeof(FunctionNode));
    fd->name = name;
    fd->parameters = parameters;
    fd->expr = expression;
    return fd;
}

FunctionNode* addFunctionNode(FunctionNode* first, FunctionNode* function) {
    FunctionNode* f = first;
    while(f->next != NULL) {
        f = f->next;
    }
    f->next = function;
    return first;
}