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
    n->reference = malloc(sizeof(Reference));
    n->reference->name = name;
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

Node* createAssignNode(char* name, Node* expr){
    Node* n = malloc(sizeof(Node));
    n->type = NT_ASSIGN;
    n->assign = malloc(sizeof(Assign));
    n->assign->name = name;
    n->assign->expression = expr;
    return n;
}

Node* createProgram(NodeList* functions, NodeList* statements, Node* ret) {
    Node* n= malloc(sizeof(Node));
    n->type = NT_PROGRAM;
    n->program = malloc(sizeof(Program));
    n->program->functions = functions;
    n->program->statements = statements;
    n->program->ret = ret;
    return n;
}

Node* createFunctionDefinition(char* name, NodeList* parameters, Node* expression) {
    Node* n = malloc(sizeof(Node));
    n->type = NT_FUNCTION_DEFINITION;
    n->functionDefinition = malloc(sizeof(FunctionDefinition));
    n->functionDefinition->name = name;
    n->functionDefinition->parameters = parameters;
    n->functionDefinition->expr = expression;
    return n;
}

Node* createParameterNode(char* name) {
    Node* n = malloc(sizeof(Node));
    n->type = NT_PARAMETER;
    n->parameter = malloc(sizeof(Parameter));
    n->parameter->name = name;
    return n;
}

NodeList* createNodeList(Node* first) {
    NodeList* list = malloc(sizeof(NodeList));
    list->node = first;
    list->next = NULL;
    return list;
}

NodeList* appendNode(NodeList* list, Node* next) {
    NodeList* last = list;
    while(last->next != NULL) {
        last = last->next;
    }
    last->next = createNodeList(next);
    return list;
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

Symbol* createAndAddSymbol(SymbolsTable* symbols, char* name, Node* value) {
    Symbol* s = malloc(sizeof(Symbol));
    s->name = name;
    s->value = value;
    if (appendSymbol(symbols, s) == SYMBOL_ALREADY_DEFINED) { 
        fprintf(stderr, "Symbol already defined: %s\n", name); 
        exit(1); 
    }
    return s;
}
