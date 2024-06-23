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

int vars = 0;

char* newVarName(){
    ++vars;
    char* name = malloc(sizeof(char) * 3);
    sprintf(name, "%d", vars);
    return name;
}

LLVMValueRef generateNumberValue(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateArithmeticExpressionValue(LLVMBuilderRef builder, Node* n);
LLVMValueRef generateValue(LLVMBuilderRef builder, Node* n);

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

// Generate a value reference for the node. Recursively generates values for left and right, if needed.
LLVMValueRef generateValue(LLVMBuilderRef builder, Node* n) {
    switch(n->type) {
        case NT_NUMBER:
            return generateNumberValue(builder, n);
        case NT_ARITHMETIC_EXPRESSION:
            return generateArithmeticExpressionValue(builder, n);
        default:
            fprintf(stderr, "Unsupported node type: %d", n->type);
            exit(1);
            break;
    }
}

void generate(Node* node, char* filename) {
    // Scaffoling begins...
    LLVMInitializeCore(LLVMGetGlobalPassRegistry());
    atexit(LLVMShutdown);
    LLVMModuleRef module = LLVMModuleCreateWithName("Calculator");
    LLVMSetSourceFileName(module, filename, strlen(filename));
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef int32Type = LLVMInt32TypeInContext(LLVMGetGlobalContext());
    LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef mainFunction = LLVMAddFunction(module, "main", mainType);
    LLVMBasicBlockRef entryBlock = LLVMAppendBasicBlock(mainFunction, "entry");
    LLVMPositionBuilderAtEnd(builder, entryBlock);
    // Scaffoling ends...

    LLVMValueRef result = generateValue(builder, node);
    LLVMBuildRet(builder, result);

    // Write begins...
    char *error = NULL;
    LLVMVerifyModule(module, LLVMAbortProcessAction, &error);
    LLVMDisposeMessage(error);
    if (LLVMWriteBitcodeToFile(module, filename) != 0) {
        fprintf(stderr, "Error writing bitcode to file. Skipping.\n");
    }

    // Print the LLVM IR to stdout
    char *irCode = LLVMPrintModuleToString(module);
    printf("%s\n", irCode);
    LLVMDisposeMessage(irCode);
    // Write ends

    chmod(filename, S_IRWXU | S_IRWXG);
}