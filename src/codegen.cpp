#include "ast.h"
#include "staff.h"
#include "symbolTable.h"
#include "llvm-utils.h"

extern ST::SymbolTable symTab;

using namespace AST;
using namespace llvm;

/* Code generation methods */

/* Block: blocks are only used at the highest of levels in our code, so we have no code to generate */
void AST::Block::codeGen(){

    //code->dump(); //if curious about the generated IR
}

void AST::Value::codeGen(){
    if(code == NULL){ //Generates code if we have none. We only do it when we have an expression or assignment
        if(type == TYPE::integer){
            int v = std::stoi(value);
            code = ConstantInt::get(IR::Context, APInt(64,v));
        } else if(type == TYPE::real) {
            auto v = std::stof(value);
            // code = ConstantFP::get(IR::Context, APFloat(v));
            code = ConstantInt::get(IR::Context, APInt(64,0));//FIXIT
        } else if(type == TYPE::boolean){
            // if(value == "verdade"){
            //     code = ConstantInt::get(IR::Context, APInt(1,1));
            // } else {
            //     code = ConstantInt::get(IR::Context, APInt(1,0));
            // }
            code = ConstantInt::get(IR::Context, APInt(64,0));//FIXIT
        } else {
            code = ConstantInt::get(IR::Context, APInt(64,0));
        }
    }
    //code->dump(); //if curious about the generated IR
}

/* Binary operation: we add to the basic block an instruction using
 * the code generated for the left and right hand sides */
void AST::BinOp::codeGen(){
   if (code == NULL) { //generates code if we have none
        if (op == OPERATION::assign){
            /*Assignments use the name of the variable and update the symbol table.*/
            Word* lvar = dynamic_cast<Word *>(left);
            right->codeGen(); /*gets right side*/
            code = IR::Builder.CreateAdd(right->code, IR::Zero, lvar->word.c_str()); /*'copies' the value to the variable*/
            symTab.updateVariable(lvar->word, code); /*Gives the new code and value to the variable in the symbol table.*/
        }
    }
}

/* Word: variables have their latest values in the symbol table, so we can get them from there */
void AST::Word::codeGen(){
    if (code == NULL){ //generates code if we have none
        code = symTab.useVariableLLVM(word);
    }
    // code->dump(); //if curious about the generated IR
}

void AST::VariableDeclaration::codeGen(){
    
    // code->dump(); //if curious about the generated IR
}

void AST::ArrayDeclaration::codeGen(){
    
    //code->dump(); //if curious about the generated IR
}

void AST::UnOp::codeGen(){

}

void AST::Coercion::codeGen() {

}

void AST::Param::codeGen() {

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
