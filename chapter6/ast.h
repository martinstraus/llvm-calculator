#ifndef GROG_AST

#include <llvm-c/Core.h>

#define GROG_AST
#define SYMBOL_ADDED 1
#define SYMBOL_ALREADY_DEFINED 0

/* AST */

/*
For this proof of concept, it seems enough to have a single type.
In future versions, different enums (for instance, one for arithmetic operations) might be wise.
*/
typedef enum NodeType {
    NT_NUMBER, NT_ARITHMETIC_EXPRESSION, NT_REFERENCE, NT_ASSIGN, NT_PROGRAM, NT_FUNCTION_DEFINITION,
    NT_PARAMETER, NT_FUNCTION_CALL
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
typedef struct FunctionDefinition FunctionDefinition;
typedef struct Parameter Parameter;
typedef struct FunctionCall FunctionCall;
typedef struct SymbolsList SymbolsList;
typedef struct SymbolsTable SymbolsTable;

struct Node {
    NodeType type;
    int number;                                 // used only when type == NT_NUMBER
    Reference* reference;                       // used only when type == NT_REFERENCE
    ArithmeticExpression* arithmeticExpression; // used only when type == NT_ARITHMETIC_EXPRESSION
    Assign* assign;                             // used only when type == NT_ASSIGN
    Program* program;                           // used only when type == NT_PROGRAM
    FunctionDefinition* functionDefinition;     // used only when type == NT_FUNCTION_DEFINTION
    Parameter* parameter;                       // used only when type == NT_PARAMETER
    FunctionCall* functionCall;                 // used only then tpye == NT_FUNCTION_CALL
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
    NodeList* functions;
    NodeList* statements;
    Node* ret;
};

struct Parameter {
    char* name;
};

struct FunctionDefinition {
    char* name;
    NodeList* parameters;
    Node* expr;
};

struct FunctionCall {
    char* name;
    NodeList* parameters;
};

Node* createIntNode(int value);
Node* createReferenceNode(char* name);
Node* createExprNode(ArithmeticOperator operator, Node* left, Node* right);
Node* createAssignNode(char* name, Node* expr);
Node* chainStatements(Node* first, Node* second);
NodeList* createNodeList(Node* first);
NodeList* appendNode(NodeList* list, Node* next);
Node* createProgram(NodeList* functions, NodeList* statements, Node* ret);
Node* createFunctionDefinition(char* name, NodeList* parameters, Node* expression);
Node* createParameterNode(char* name);
Node* createFunctionCallNode(char* name, NodeList* parameters);

/* SYMBOLS TABLE*/

// Each symbol is something defined in assignment statements.
typedef enum SymbolType { ST_PARAMETER, ST_CONSTANT, ST_FUNCTION } SymbolType;

typedef struct Symbol {
    SymbolType type;
    char* name;
    Node* value;
    LLVMValueRef ref;
    LLVMTypeRef functionType;
} Symbol;

struct SymbolsList {
    Symbol* symbol;
    SymbolsList* next;
};
 
struct SymbolsTable {
    SymbolsList* symbols;
    SymbolsTable* parent;
};

SymbolsTable* createSymbolsTable();
// Returns NULL if the symbol is not in the table.
Symbol* findSymbol(SymbolsTable* table, char* name);
// Returns 0 if the symbol is not in the table, != 0 if it is.
int containsSymbol(SymbolsTable* table, char* name);
// Appends a symbol to the table.
int appendSymbol(SymbolsTable* table, Symbol* symbol);
Symbol* createAndAddSymbol(SymbolsTable* symbols, SymbolType type, char* name, Node* value);

#endif