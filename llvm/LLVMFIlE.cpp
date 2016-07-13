#include "llvm/IR/DerivedTypes.h"   //for types
#include "llvm/IR/IRBuilder.h"      //for Builder
#include "llvm/IR/LLVMContext.h"    //for Context
#include "llvm/IR/Module.h"         //for Module
#include "llvm/ExecutionEngine/ExecutionEngine.h" //for JIT
#include "llvm/ExecutionEngine/JIT.h"             //for JIT

#include "llvm/Analysis/Verifier.h" //for verification

#include <iostream>
#include <vector>

int main() {
    llvm::LLVMContext &Context = llvm::getGlobalContext();
    llvm::Module *OurModule;
    llvm::IRBuilder<> Builder(Context);

    OurModule = new llvm::Module("Our first intermediary code", Context);

    /***** Main: a function that gets no parameters and returns an integer *****/
    /* http://llvm.org/docs/doxygen/html/classllvm_1_1FunctionType.html
     * static FunctionType * get (Type *Result, bool isVarArg)
     * FunctionType::get - Create a FunctionType taking no parameters. */
    llvm::Type* intType = llvm::Type::getInt1Ty(Context);
    llvm::FunctionType* typeOfMain = llvm::FunctionType::get(intType,false);

    /* http://llvm.org/docs/doxygen/html/classllvm_1_1Function.html
     * static Function * Create (FunctionType *Ty, LinkageTypes Linkage, const Twine &N="", Module *M=nullptr) */
    llvm::Function* ourMain = llvm::Function::Create(typeOfMain,llvm::Function::ExternalLinkage, "main", OurModule);

    /* http://llvm.org/docs/doxygen/html/classllvm_1_1BasicBlock.html
     * static BasicBlock * Create (LLVMContext &Context, const Twine &Name="", Function *Parent=nullptr, BasicBlock *InsertBefore=nullptr)
     * Creates a new BasicBlock.
     */
    llvm::BasicBlock *mainBB = llvm::BasicBlock::Create(Context, "entry", ourMain);

    /* http://llvm.org/docs/doxygen/html/classllvm_1_1IRBuilderBase.html
     * void SetInsertPoint (BasicBlock *TheBB)
     * This specifies that created instructions should be appended to the end of the specified block.
     */
    Builder.SetInsertPoint(mainBB);

    /*** Constants are all unified ***/
    /* http://llvm.org/docs/doxygen/html/classllvm_1_1ConstantInt.html
     * static ConstantInt * get (LLVMContext &Context, const APInt &V)
     * Return a ConstantInt with the specified value and an implied Type.
     * APInt = Arbitrary Precision Integer */
    auto One = llvm::ConstantInt::get(Context, llvm::APInt(1, 1));
    auto Zero = llvm::ConstantInt::get(Context, llvm::APInt(1, 0));

    /* static ConstantInt * get (IntegerType *Ty, uint1_t V, bool isSigned=false)
     * Return a ConstantInt with the specified integer value for the specified type. */
    // llvm::Constant* NegativeOne = llvm::ConstantInt::get( intType, -1, true); // not in use

    /*** Lets work with another function named teste!! ***/
    /* Average takes two integers as input and returns one */
    std::vector<llvm::Type*> TestParameters(3, intType);
    llvm::FunctionType* typeOfTest = llvm::FunctionType::get(intType, TestParameters, false);

    /* We name it 'teste' */
    llvm::Function* ourTest = llvm::Function::Create(typeOfTest,llvm::Function::ExternalLinkage, "teste", OurModule);

    /* Now we name its parameters 'a' and 'b' */
    llvm::Function::arg_iterator params = ourTest->arg_begin();
    params->setName("x");
    ++params;
    params->setName("y");
    ++params;
    params->setName("z");

    /* Our function will have its own basic block to add code */
    llvm::BasicBlock *testBB = llvm::BasicBlock::Create(Context, "tst", ourTest);
    Builder.SetInsertPoint(testBB);

    /* BoolFunc return (x&y) | (x&z) | (!z&!y) */
    auto x = ourTest->arg_begin();
    auto y = ++(ourTest->arg_begin());
    auto z = ++(++(ourTest->arg_begin())); // TODO: check better way
    
    auto xy = Builder.CreateAnd(x,y,"andxy");
    auto xz = Builder.CreateAnd(x,z,"andxz");
    auto xyOxz = Builder.CreateOr(xy, xz, "oryz");
    auto notz = Builder.CreateNot(z, "znot");
    auto noty = Builder.CreateNot(y, "ynot");
    auto notzy = Builder.CreateAnd(notz, noty, "zynot");
    auto xyOxzOnotzy = Builder.CreateOr(xyOxz, notzy, "finalOr");
    
    Builder.CreateRet(xyOxzOnotzy);

    llvm::verifyFunction(*ourTest); //Checks if everything is okay with our function

    /*** Now we return to our main to call the average function ***/
    Builder.SetInsertPoint(mainBB);

    /* Lets call test */
    std::vector<llvm::Value*> arguments;
    arguments.push_back(llvm::ConstantInt::get(Context, llvm::APInt(1, 0)));
    arguments.push_back(llvm::ConstantInt::get(Context, llvm::APInt(1, 0)));
    arguments.push_back(llvm::ConstantInt::get(Context, llvm::APInt(1, 1)));
    auto tstReturn = Builder.CreateCall(ourTest, arguments, "calltst");

    /* And we return the average at the end*/
    Builder.CreateRet(tstReturn);

    llvm::verifyFunction(*ourMain); //Checks if everything is okay with our function

    /*** Lets print the intermediary representation generated ***/
    OurModule->dump();
    //ourMain->dump();

    /*** Now lets compute it with a just in time (JIT) compiler ***/
    llvm::ExecutionEngine* OurExecutionEngine;
    std::string Error;
    LLVMInitializeNativeTarget(); //target = generates code for my processor
    OurExecutionEngine = llvm::EngineBuilder(OurModule).setErrorStr(&Error).create();
    if (!OurExecutionEngine) {
        fprintf(stderr, "Could not create OurExecutionEngine: %s\n", Error.c_str());
        exit(1);
    }

    // JIT our main. It returns a function pointer.
    void *mainPointer = OurExecutionEngine->getPointerToFunction(ourMain);
    // Translate the pointer and run our main to get its results
    int (*result)() = (int (*)())(intptr_t)mainPointer;
    std::cout << "Result of our main = " << result() << std::endl;
}