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
    st->symbols = malloc(sizeof(SymbolsList));
    st->symbols->next = NULL;
    st->parent = NULL;
    return st; 
}

Symbol* findSymbol(SymbolsTable* table, char* name) {
    SymbolsTable* t = table;
    SymbolsList* sl = NULL;
    
    while (t != NULL) {
        SymbolsList* sl = t->symbols;
        while (sl != NULL) {
            if (sl->symbol != NULL && strcmp(name, sl->symbol->name) == 0) {
                return sl->symbol;
            }   
            sl = sl->next;
        }
        t = t->parent;
    }
    return NULL;
}

// Returns false if the symbol is not in the table.
int containsSymbol(SymbolsTable* table, char* name) {
    return findSymbol(table, name) != NULL;
}

int appendSymbol(SymbolsTable* table, Symbol* symbol) {
    SymbolsList* sl = table->symbols;

    // Go to the end of the list.
    /*while (sl != NULL && sl->symbol != NULL) {
        if (strcmp(symbol->name, sl->symbol->name) == 0) {
            return SYMBOL_ALREADY_DEFINED;
        }
        sl = sl->next;
    }*/

    // Go to the last symbols list.
    while (sl->next != NULL) {
        sl = sl->next;
    }
    // Here, sl should always be != NULL
    
    if (sl->symbol == NULL) {
        sl->symbol = symbol;
    } else {
        sl->next = malloc(sizeof(SymbolsList));
        sl->next->symbol = symbol;
        sl->next->next = NULL;        
    }
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
