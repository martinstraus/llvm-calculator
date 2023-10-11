#ifndef GROG_AST
#define GROG_AST

/*
For this proof of concept, it seems enough to have a single type.
In future versions, different enums (for instance, one for arithmetic operations) might be wise.
*/
typedef enum NodeType {
    NT_NUMBER, NT_REFERENCE, NT_ADD, NT_SUB, NT_MUL, NT_DIV, NT_ASSIGN
} NodeType;

typedef struct Node {
    NodeType type;
    int number;         // used only when type == NT_NUMBER
    char* name;         // used only when type == NT_REFERENCE
    struct Node *left;  // used only when type == on of the arithmetic operations
    struct Node *right; // used only when type == on of the arithmetic operations
    struct Node *expr;  // used only when type == NT_ASSIGN
} Node;

Node* createIntNode(int value);
Node* createReferenceNode(char* name);
Node* createExprNode(NodeType type, Node* left, Node* right);
Node* createAssignNode(char* name, Node* expr);

typedef struct Program {
    Node* assign;   // This should be changed to a list of statements.
    Node* ret;      // The return expression.
} Program;

Program* createProgram(Node* assign, Node* ret);

#endif