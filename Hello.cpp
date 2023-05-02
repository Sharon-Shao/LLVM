//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "hello"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/Value.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"

using namespace llvm;



namespace {
struct Hello :  public FunctionPass, public CallInst
{

        /** Constructor. */
	static char ID;                           
	Hello() : FunctionPass(ID) {}

        //DEFINE_INTPASS_ANALYSIS_ADJUSTMENT(PointerAnalysisPass);

        /**
         * @brief Runs this pass on the given function.
         * @param [in,out] func The function to analyze
         * @return true if the function was modified; false otherwise
        */
    
    
        virtual bool runOnFunction(llvm::Function &F){
            errs() << "Hello: " ;
            errs().write_escaped(F.getName())<< "\n";
            
            for (Function &F : M) {
              for (BasicBlock &BB : F) {
                for (Instruction &I : BB) {
                  if (auto *CI = dyn_cast<CallInst>(&I)) {
                    Function *CalledFunc = CI->getCalledFunction();
                    if (CalledFunc != nullptr) {
                      errs() << "Found function call: " << CalledFunc->getName() << "\n";
                      for (unsigned int i = 0; i < CI->getNumArgOperands(); i++) {
                        Value *Arg = CI->getArgOperand(i);
                        errs() << "Argument " << i << ": " << *Arg << "\n";
                      }
                    }
                  }
                }
              }
            }
            return false;
        }
    

    };
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass", false, false);



