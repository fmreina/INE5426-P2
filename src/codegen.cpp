#include "ast.h"
#include "symbolTable.h"
#include "llvm-utils.h"

extern ST::SymbolTable symbolTable;

using namespace AST;
using namespace llvm;

/* Code generation methods */

/* Block: blocks are only used at the highest of levels in our code, so we have no code to generate */
void AST::Block::codeGen(){
    
    //code->dump(); //if curious about the generated IR
}

void AST::Value::codeGen(){
    
    //code->dump(); //if curious about the generated IR
}

/* Binary operation: we add to the basic block an instruction using
 * the code generated for the left and right hand sides */
void AST::BinOp::codeGen(){
   
}

/* Word: variables have their latest values in the symbol table, so we can get them from there */
void AST::Word::codeGen(){
    
    // code->dump(); //if curious about the generated IR
}

void AST::VariableDeclaration::codeGen(){
    
    // code->dump(); //if curious about the generated IR
}

void AST::ArrayDeclaration::codeGen(){
    
    //code->dump(); //if curious about the generated IR
}

void AST::FunctionDeclaration::codeGen(){
    
    //code->dump(); //if curious about the generated IR
}

void AST::FunctionDefinition::codeGen(){
    
    //code->dump(); //if curious about the generated IR
}

void AST::IfBlock::codeGen(){
    
    //code->dump(); //if curious about the generated IR
}

void AST::WhileBlock::codeGen(){
    
    //code->dump(); //if curious about the generated IR
}

void AST::FromTil_Block::codeGen(){
    
    //code->dump(); //if curious about the generated IR
}
