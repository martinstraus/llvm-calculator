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

extern SymbolsTable* symbols; // Defined in parser.y

int vars = 0;

char* newVarName(){
    ++vars;
    char* name = malloc(sizeof(char) * 3);
    sprintf(name, "tmp%d", vars);
    return name;
}

// We declare these functions here, so the declarations can reference each other.
LLVMValueRef generateNumberValue(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateArithmeticExpressionValue(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateReference(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateValue(LLVMBuilderRef builder, Node* n);
void generateStatement(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateReturn(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateProgram(LLVMBuilderRef builder, Program* program);

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
    return LLVMBuildLoad2(builder, LLVMInt32Type(), symbol->ref, newVarName());
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
    Symbol* symbol = findSymbol(symbols, n->assign->name);
    if (symbol == NULL) {
        fprintf(stderr, "Couldn't find symbol %s.\n", n->assign->name);
        exit(1);
    }
    symbol->ref = var;
}

LLVMValueRef generateReturn(LLVMBuilderRef builder, Node* n) {
    LLVMValueRef ret = generateValue(builder, n);
    return LLVMBuildRet(builder, ret);
}

LLVMValueRef generateProgram(LLVMBuilderRef builder, Program* program) {
    Node* a = program->assign;
    while (a != NULL) {
        generateStatement(builder, a);
        a = a->next;
    }
    return generateReturn(builder, program->ret);
}

void generate(Program* root, char* sourcefile, char* outputfile) {
    // Scaffoling begins...
    LLVMInitializeCore(LLVMGetGlobalPassRegistry());
    atexit(LLVMShutdown);
    LLVMModuleRef module = LLVMModuleCreateWithName("Calculator");
    LLVMSetSourceFileName(module, sourcefile, strlen(sourcefile));
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef int32Type = LLVMInt32TypeInContext(LLVMGetGlobalContext());
    LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef mainFunction = LLVMAddFunction(module, "main", mainType);
    LLVMBasicBlockRef entryBlock = LLVMAppendBasicBlock(mainFunction, "entry");
    LLVMPositionBuilderAtEnd(builder, entryBlock);
    // Scaffoling ends...

    generateProgram(builder, root);
    
    // Print the LLVM IR to stdout
    char *irCode = LLVMPrintModuleToString(module);
    printf("%s\n", irCode);
    LLVMDisposeMessage(irCode);

    // Write begins...
    char *error = NULL;
    if (LLVMVerifyModule(module, LLVMAbortProcessAction, &error)) {
        fprintf(stderr, "Error verifying module: %s\n", error);
    }
    LLVMDisposeMessage(error);

    if (LLVMWriteBitcodeToFile(module, outputfile)) {
        fprintf(stderr, "Error writing bitcode to file. Skipping.\n");
        exit(2);
    }
    // Write ends

    chmod(outputfile, S_IRWXU | S_IRWXG);
}