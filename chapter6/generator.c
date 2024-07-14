#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "ast.h"

#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

#define false 0
#define true 1

extern SymbolsTable* symbols; // Defined in compiler.c  

int vars = 0;

char* newVarName(){
    ++vars;
    char* name = malloc(sizeof(char) * 3);
    sprintf(name, "tmp%d", vars);
    return name;
}

LLVMValueRef generateNumberValue(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateArithmeticExpressionValue(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateReference(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateValue(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateFunctionCall(LLVMBuilderRef builder, Node* n);
void generateStatement(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateReturn(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateProgram(LLVMBuilderRef builder, LLVMModuleRef module, Node* program);

LLVMValueRef generateNumberValue(LLVMBuilderRef builder, Node* n) {
    return LLVMConstInt(LLVMInt32Type(), n->number, false);
}

LLVMValueRef generateArithmeticExpressionValue(LLVMBuilderRef builder, Node* n) {
    LLVMValueRef left = generateValue(builder, n->arithmeticExpression->left);
    LLVMValueRef right = generateValue(builder, n->arithmeticExpression->right);
    switch (n->arithmeticExpression->operator) {
        case AO_ADD:
            return LLVMBuildAdd(builder, left, right, newVarName());
        case AO_SUB:
            return LLVMBuildSub(builder, left, right, newVarName());
        case AO_MUL:
            return LLVMBuildMul(builder, left, right, newVarName());
        case AO_DIV:
            return LLVMBuildExactUDiv(builder, left, right, newVarName());
        default:
            fprintf(stderr, "Unsupported operator: %d", n->type);
            exit(1);
            break;
    }
}

LLVMValueRef generateReference(LLVMBuilderRef builder, Node* n) {
    Symbol* symbol = findSymbol(symbols, n->reference->name);
    if (symbol == NULL) {
       fprintf(stderr, "Couldn't find symbol %s.\n", n->reference->name);
       exit(1);
    }
    if (symbol->type == ST_PARAMETER) {
        return symbol->ref;
    } else {
        return LLVMBuildLoad2(builder, LLVMInt32Type(), symbol->ref, newVarName());
    }
}

// Generate a value reference for the node. Recursively generates values for left and right, if needed.
LLVMValueRef generateValue(LLVMBuilderRef builder, Node* n) {
    switch(n->type) {
        case NT_NUMBER:
            return generateNumberValue(builder, n);
        case NT_ARITHMETIC_EXPRESSION:
            return generateArithmeticExpressionValue(builder, n);
        case NT_REFERENCE:
            return generateReference(builder, n);
        case NT_FUNCTION_CALL:
            return generateFunctionCall(builder, n);
        default:
            fprintf(stderr, "Unsupported node type: %d", n->type);
            exit(1);
            break;
    }
}

void generateStatement(LLVMBuilderRef builder, Node* n) {
    if (n->type != NT_ASSIGN) {
        fprintf(stderr, "Unsupported statement type: %d\n", n->type);
    }
    LLVMValueRef var = LLVMBuildAlloca(builder, LLVMInt32Type(), n->assign->name);
    LLVMValueRef val = generateValue(builder, n->assign->expression);
    LLVMBuildStore(builder, val, var);    
    Symbol* symbol = createAndAddSymbol(symbols, ST_CONSTANT, n->assign->name, n);
    symbol->ref = var;
}

LLVMValueRef generateReturn(LLVMBuilderRef builder, Node* n) {
    return LLVMBuildRet(builder, generateValue(builder, n));
}

// Not fully implemented yet
LLVMValueRef generateFunction(LLVMBuilderRef builder, LLVMModuleRef module, Node* f) {

    // Get the types of the parameters. We need them to create the function.
    // Perhaps not the most beautiful code ever produced...
    LLVMTypeRef int32Type = LLVMInt32Type();
    NodeList* p = f->functionDefinition->parameters;
    int paramCount = 0;
    while(p != NULL) {
        paramCount++;
        p=p->next;
    }
    LLVMTypeRef* parametersTypes = malloc(sizeof(LLVMTypeRef) * paramCount);
    for(int i = 0; i < paramCount; i++) {
        parametersTypes[i] = int32Type;
    }

    // Create the function
    LLVMTypeRef functionType = LLVMFunctionType(int32Type, parametersTypes, paramCount, false);
    LLVMValueRef functionValue = LLVMAddFunction(module, f->functionDefinition->name, functionType);

    // Create the symbols table with the parameteres.
    p = f->functionDefinition->parameters;
    SymbolsTable* st = createSymbolsTable();
    st->parent = symbols;
    symbols = st;
    int paramIndex = 0;
    while (p != NULL) {        
        Symbol* symbol = createAndAddSymbol(symbols, ST_PARAMETER, p->node->parameter->name, p->node);
        symbol->ref = LLVMGetParam(functionValue, paramIndex);
        p = p->next;
    }

    // Now we can build the function's body.
    LLVMBasicBlockRef entryBlock = LLVMAppendBasicBlock(functionValue, "entry");
    LLVMPositionBuilderAtEnd(builder, entryBlock);
    LLVMValueRef value = generateValue(builder, f->functionDefinition->expr);
    LLVMValueRef ret = LLVMBuildRet(builder, value);
    
    symbols = symbols->parent;
    Symbol* s = malloc(sizeof(Symbol));
    s->type = ST_FUNCTION;
    s->name = f->functionDefinition->name;
    s->value = f;
    s->ref = functionValue;
    s->functionType = functionType;
    if (appendSymbol(symbols, s) == SYMBOL_ALREADY_DEFINED) {
        fprintf(stderr, "Symbol '%s' already defined.", f->functionDefinition->name);
        exit(1);
    }
    return ret;
}

LLVMValueRef generateMainFunction(LLVMBuilderRef builder, LLVMModuleRef module, Node* n) {
    LLVMTypeRef int32Type = LLVMInt32Type();
    LLVMTypeRef functionType = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef function = LLVMAddFunction(module, "main", functionType);
    LLVMBasicBlockRef entryBlock = LLVMAppendBasicBlock(function, "entry");
    LLVMPositionBuilderAtEnd(builder, entryBlock);

    NodeList* statement = n->program->statements;
    while (statement != NULL) {
        generateStatement(builder, statement->node);
        statement = statement->next;
    }
    return generateReturn(builder, n->program->ret);
}

LLVMValueRef generateProgram(LLVMBuilderRef builder, LLVMModuleRef module, Node* n) {
    NodeList* f = n->program->functions;
    while(f != NULL) {
        generateFunction(builder, module, f->node);
        f = f->next;
    }

    generateMainFunction(builder, module, n);
}

LLVMValueRef generateFunctionCall(LLVMBuilderRef builder, Node* n) {
    Symbol* s = findSymbol(symbols, n->functionCall->name);
    if (s == NULL) {
        fprintf(stderr, "Function '%s' not found.\n", n->functionCall->name);
        exit(1);
    }

    // Build the array with parameters;
    int paramCount = 0;
    NodeList* paramList = n->functionCall->parameters;
    while (paramList != NULL) {
        paramCount++;
        paramList = paramList->next;
    }
    LLVMValueRef* parameters = malloc(sizeof(LLVMValueRef) * paramCount);
    paramList = n->functionCall->parameters;
    for (int i = 0;i < paramCount; i++) {
        parameters[i] = generateValue(builder, paramList->node);
        paramList = paramList->next;
    }

    return LLVMBuildCall2(builder, s->functionType, s->ref, parameters, paramCount, n->functionCall->name);
}

void generate(Node* root, char* sourcefile, char* outputfile) {
    // Scaffoling begins...
    LLVMInitializeCore(LLVMGetGlobalPassRegistry());
    atexit(LLVMShutdown);
    LLVMModuleRef module = LLVMModuleCreateWithName("Calculator");
    LLVMSetSourceFileName(module, sourcefile, strlen(sourcefile));
    LLVMBuilderRef builder = LLVMCreateBuilder();
    // Scaffoling ends...

    generateProgram(builder, module, root);
    
    // Verify module
    char *error = NULL;
    if (LLVMVerifyModule(module, LLVMAbortProcessAction, &error)) {
        fprintf(stderr, "Error verifying module: %s\n", error);
    }
    LLVMDisposeMessage(error);

    // Print the LLVM IR to stdout
    char *irCode = LLVMPrintModuleToString(module);
    printf("%s\n", irCode);
    LLVMDisposeMessage(irCode);

    // Write begins...
    if (LLVMWriteBitcodeToFile(module, outputfile)) {
        fprintf(stderr, "Error writing bitcode to file. Skipping.\n");
        exit(2);
    }
    // Write ends

    chmod(outputfile, S_IRWXU | S_IRWXG);
}