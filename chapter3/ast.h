#ifndef GROG_AST
#define GROG_AST

/*
For this proof of concept, it seems enough to have a single type.
In future versions, different enums (for instance, one for arithmetic operations) might be wise.
*/
typedef enum NodeType {
    NT_NUMBER, NT_ADD, NT_SUB, NT_MUL, NT_DIV
} NodeType;

typedef struct Node {
    NodeType type;
    int number; // used only when type == NUMBER
    struct Node *left; // used only when type == on of the arithmetic operations
    struct Node *right; // used only when type == on of the arithmetic operations
} Node;

Node* createIntNode(int value);
Node* createExprNode(NodeType type, Node* left, Node* right);

#endif