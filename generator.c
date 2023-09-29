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

    LLVMValueRef result = LLVMConstInt(LLVMInt32Type(), 0, false);
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