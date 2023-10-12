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

Node* chainStatements(Node* first, Node* second) {
    first->next = second;
    return first;
}

Program* createProgram(Node* assign, Node* ret) {
    Program* p = malloc(sizeof(Program));
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
    while (t != NULL && !strcmp(name, t->symbol->name)) {
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

