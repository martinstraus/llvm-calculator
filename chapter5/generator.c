#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

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

// Generate a value reference for the node. Recursively generates values for left and right, if needed.
LLVMValueRef generateValue(LLVMBuilderRef builder, Node* n) {
    if (n->type == NT_NUMBER) {
        return LLVMConstInt(LLVMInt32Type(), n->number, false);
    } else if (n->type == NT_REFERENCE) {
        return LLVMConstInt(LLVMInt32Type(), 0, false);
    } else if (n->type == NT_ASSIGN) {
        LLVMValueRef var = LLVMBuildAlloca(builder, LLVMInt32Type(), n->name);
        LLVMValueRef val = generateValue(builder, n->expr);
        LLVMBuildStore(builder, val, var);
        return var;
    } else {
        LLVMValueRef left = generateValue(builder, n->left);
        LLVMValueRef right = generateValue(builder, n->right);
        switch (n->type) {
            case NT_ADD:
                return LLVMBuildAdd(builder, left, right, newVarName());
                break;
            case NT_SUB:
                return LLVMBuildSub(builder, left, right, newVarName());
                break;
            case NT_MUL:
                return LLVMBuildMul(builder, left, right, newVarName());
                break;
            case NT_DIV:
                return LLVMBuildExactUDiv(builder, left, right, newVarName());
                break;
            default:
                fprintf(stderr, "Unsupported operator: %d", n->type);
                exit(1);
                break;
        }
    } 
}

LLVMValueRef generateProgram(LLVMBuilderRef builder, Program* program) {
    Node* a = program->assign;
    while (a != NULL) {
        generateValue(builder, program->assign);
        a = a->next;
    }
    return generateValue(builder, program->ret);;
}

void generate(Program* root, char* filename) {
    // Scaffoling begins...
    LLVMInitializeCore(LLVMGetGlobalPassRegistry());
    atexit(LLVMShutdown);
    LLVMModuleRef module = LLVMModuleCreateWithName("Grog");
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMTypeRef int32Type = LLVMInt32TypeInContext(LLVMGetGlobalContext());
    LLVMTypeRef mainType = LLVMFunctionType(int32Type, NULL, 0, 0);
    LLVMValueRef mainFunction = LLVMAddFunction(module, "main", mainType);
    LLVMBasicBlockRef entryBlock = LLVMAppendBasicBlock(mainFunction, "entry");
    LLVMPositionBuilderAtEnd(builder, entryBlock);
    // Scaffoling ends...

    LLVMValueRef result = generateProgram(builder, root);
    LLVMBuildRet(builder, result);

    // Write begins...
    char *error = NULL;
    if (LLVMVerifyModule(module, LLVMAbortProcessAction, &error)) {
        fprintf(stderr, "Error verifying module: %s\n", error);
    }
    LLVMDisposeMessage(error);
    
    if (LLVMWriteBitcodeToFile(module, filename)) {
        fprintf(stderr, "Error writing bitcode to file. Skipping.\n");
        exit(2);
    }
    // Write ends

    chmod(filename, S_IRWXU | S_IRWXG);
}