#ifndef GROG_AST

#include <llvm-c/Core.h>

#define GROG_AST

/* AST */

/*
For this proof of concept, it seems enough to have a single type.
In future versions, different enums (for instance, one for arithmetic operations) might be wise.
*/
typedef enum NodeType {
    NT_NUMBER, NT_ARITHMETIC_EXPRESSION, NT_REFERENCE, NT_ASSIGN, NT_PROGRAM
} NodeType;

typedef enum ArithmeticOperator {
    AO_ADD, AO_SUB, AO_MUL, AO_DIV
} ArithmeticOperator;

typedef struct ArithmeticExpression ArithmeticExpression;
typedef struct Node Node;
typedef struct Assign Assign;
typedef struct Reference Reference;
typedef struct Program Program;
typedef struct NodeList NodeList;

struct Node {
    NodeType type;
    int number;                                 // used only when type == NT_NUMBER
    Reference* reference;                       // used only when type == NT_REFERENCE
    ArithmeticExpression* arithmeticExpression; // used only when type == NT_ARITHMETIC_EXPRESSION
    Assign* assign;                             // used only when type == NT_ASSIGN
    Program* program;                           // used only when type == NT_PROGRAM
};

struct NodeList {
    Node* node;
    NodeList* next;
};

struct ArithmeticExpression {
    ArithmeticOperator operator;
    Node *left;
    Node *right;
};

struct Assign {
    char* name;
    Node* expression;
};

struct Reference {
    char* name;
};

struct Program {
    NodeList* statements;
    Node* ret;
};

Node* createIntNode(int value);
Node* createReferenceNode(char* name);
Node* createExprNode(ArithmeticOperator operator, Node* left, Node* right);
Node* createAssignNode(char* name, Node* expr);
Node* chainStatements(Node* first, Node* second);
NodeList* createNodeList(Node* first);
NodeList* appendNode(NodeList* list, Node* next);
Node* createProgram(NodeList* statements, Node* ret);

/* SYMBOLS TABLE*/

// Each symbol is something defined in assignment statements.
typedef struct Symbol {
    char* name;
    Node* value;
    LLVMValueRef ref;
} Symbol;
 
/*
This is the most naive symbols table possible: a linked list of symbols. 
Search is horribly slow with many symbols.
*/
typedef struct SymbolsTable {
    Symbol* symbol;
    struct SymbolsTable* next;
} SymbolsTable;

SymbolsTable* createSymbolsTable();
// Returns NULL if the symbol is not in the table.
Symbol* findSymbol(SymbolsTable* table, char* name);
// Returns 0 if the symbol is not in the table, != 0 if it is.
int containsSymbol(SymbolsTable* table, char* name);
// Appends a symbol to the table.
int appendSymbol(SymbolsTable* table, Symbol* symbol);
void createAndAddSymbol(SymbolsTable* symbols, char* name, Node* value);

#endif