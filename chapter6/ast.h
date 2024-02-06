#ifndef GROG_AST

#include <llvm-c/Core.h>

#define GROG_AST

/* AST */

/*
For this proof of concept, it seems enough to have a single type.
In future versions, different enums (for instance, one for arithmetic operations) might be wise.
*/
typedef enum NodeType {
    NT_NUMBER, NT_REFERENCE, NT_ADD, NT_SUB, NT_MUL, NT_DIV, NT_ASSIGN, NT_FUNCTION_DEF
} NodeType;


typedef struct Node {
    NodeType type;
    int number;         // used only when type == NT_NUMBER
    char* name;         // used only when type == NT_REFERENCE
    struct Node *left;  // used only when type == on of the arithmetic operations
    struct Node *right; // used only when type == on of the arithmetic operations
    struct Node *expr;  // used only when type == NT_ASSIGN or NT_RETURN
    struct Node *next;  // used only when type == NT_ASSIGN, in order to implement a list of assignments.
    struct FunctionDef* functionDef;
} Node;

Node* createIntNode(int value);
Node* createReferenceNode(char* name);
Node* createExprNode(NodeType type, Node* left, Node* right);
Node* createAssignNode(char* name, Node* expr);
Node* chainStatements(Node* first, Node* second);

typedef struct Program {
    Node* assign;   // This should be changed to a list of statements.
    Node* ret;      // The return expression.
} Program;

Program* createProgram(Node* assign, Node* ret);

typedef struct ParameterDef {
    char* name;
} ParameterDef;

typedef struct ParameterDefList {
    ParameterDef* value;
    struct ParameterDefList* next;
} ParameterDefList;

typedef struct FunctionNode {
    char* name;
    ParameterDefList* parameters;
    Node* expr;
} FunctionNode;

ParameterDef* createParameterDef(char* name);
FunctionNode* createFunctionNode(char* name, ParameterDefList* parameters, Node* expression);
ParameterDefList* appendParameterDef(ParameterDefList* list, ParameterDef* parameter);
ParameterDefList* createParameterDefList(ParameterDef* first);

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