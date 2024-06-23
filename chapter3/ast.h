#ifndef GROG_AST
#define GROG_AST

/*
For this proof of concept, it seems enough to have a single type.
In future versions, different enums (for instance, one for arithmetic operations) might be wise.
*/
typedef enum NodeType {
    NT_NUMBER, NT_ARITHMETIC_EXPRESSION
} NodeType;

typedef enum ArithmeticOperator {
    AO_ADD, AO_SUB, AO_MUL, AO_DIV
} ArithmeticOperator;

typedef struct ArithmeticExpression ArithmeticExpression;
typedef struct Node Node;

// This might not be the most efficient structure, but it's simple to understand.
struct Node {
    NodeType type;
    int number;
    ArithmeticExpression* arithmeticExpression;
};

struct ArithmeticExpression {
    ArithmeticOperator operator;
    Node *left;
    Node *right;
};

Node* createIntNode(int value);
Node* createExprNode(ArithmeticOperator operator, Node* left, Node* right);

#endif