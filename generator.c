#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "grog_ast.h"

#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

#define false 0
#define true 1

void generate(Node* node, char* filename) {
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

    LLVMValueRef zero = LLVMConstInt(LLVMInt32Type(), 0, false);
    LLVMValueRef result = zero;
    Node* n = node;
    while (n != NULL) {
        if (n->type == NT_NUMBER) {
            result = LLVMConstInt(LLVMInt32Type(), n->number, false);
        } else {
            LLVMValueRef left = NULL;
            LLVMValueRef right = NULL;
            if (n->left->type == NT_NUMBER) {
                left = LLVMConstInt(LLVMInt32Type(), n->left->number, false);
            }
            if (n->right->type == NT_NUMBER) {
                right = LLVMConstInt(LLVMInt32Type(), n->right->number, false);
            }
            switch (n->type) {
                case NT_ADD:
                    result = LLVMBuildAdd(builder, left, right, "result");
            }
        }
        n = NULL;
    }
    LLVMBuildRet(builder, result);

    // Write begins...
    char *error = NULL;
    LLVMVerifyModule(module, LLVMAbortProcessAction, &error);
    LLVMDisposeMessage(error);
    if (LLVMWriteBitcodeToFile(module, filename) != 0) {
        fprintf(stderr, "Error writing bitcode to file. Skipping.\n");
    }
    // Write ends

    chmod(filename, S_IRWXU | S_IRWXG);
}