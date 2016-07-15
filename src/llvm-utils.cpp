#include "llvm-utils.h"
#include <iostream>

using namespace llvm;
using namespace IR;

void IR::codeGenSetup(){
    //Starts module
    OurModule = new Module("Compilador de código intermediário", Context);
    //Set up 'main'
    /* http://llvm.org/docs/doxygen/html/classllvm_1_1FunctionType.html
     * static FunctionType * get (Type *Result, bool isVarArg)
     * FunctionType::get - Create a FunctionType taking no parameters. */
    FunctionType* typeOfMain = FunctionType::get(intType,false);
    
    /* http://llvm.org/docs/doxygen/html/classllvm_1_1Function.html
     * static Function * Create (FunctionType *Ty, LinkageTypes Linkage, const Twine &N="", Module *M=nullptr) */
    ourMain = Function::Create(typeOfMain,Function::ExternalLinkage, "compiler", OurModule);
   
    //Set up the initial basic block
    /* http://llvm.org/docs/doxygen/html/classllvm_1_1BasicBlock.html
     * static BasicBlock * Create (LLVMContext &Context, const Twine &Name="", Function *Parent=nullptr, BasicBlock *InsertBefore=nullptr)
     * Creates a new BasicBlock.
     */
    mainBB = BasicBlock::Create(Context, "start", ourMain);
    
    /* http://llvm.org/docs/doxygen/html/classllvm_1_1IRBuilderBase.html
     * void SetInsertPoint (BasicBlock *TheBB)
     * This specifies that created instructions should be appended to the end of the specified block.
     */
     Builder.SetInsertPoint(mainBB);

}
//void codeGenEnd(Value* value){
void IR::codeGenEnd(Value* endvalue){
    Builder.CreateRet(endvalue);
    verifyFunction(*ourMain);//Checks if everything is okay with our function
    std::cout << "\n####################\n";
    /*** Lets print the intermediary representation generated ***/
    OurModule->dump();
    std::cout << "####################\n";
}

