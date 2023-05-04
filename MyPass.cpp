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

#define DEBUG_TYPE "mypass"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Transforms/Utils/Cloning.h"

using namespace llvm;



namespace {
struct MyPass :  public FunctionPass
{

        /** Constructor. */
	static char ID;                           
	MyPass() : FunctionPass(ID) {}

        //DEFINE_INTPASS_ANALYSIS_ADJUSTMENT(PointerAnalysisPass);

        /**
         * @brief Runs this pass on the given function.
         * @param [in,out] func The function to analyze
         * @return true if the function was modified; false otherwise
        */
        virtual bool runOnFunction(llvm::Function &F){
		for (Function::iterator BB = F.begin(), E = F.end(); BB != E; ++BB) { // iterate over all the basic blocks in the function
 		      for (BasicBlock::iterator I = BB->begin(), E = BB->end(); I != E; ++I) { // iterate over all the instructions in the basic block 
				if (CallInst *CI = dyn_cast<CallInst>(I)) { // tries to cast all the instruction into a CallInst
					for (unsigned i = 0; i < CI->getNumArgOperands();i++) { //iterates for every argument in the instruction
                    				Value *argC = CI->getArgOperand(i);
						if (isa<Constant>(argC)){ //checks if the actual is a constant int
							ValueToValueMapTy Vmap;
							ClonedCodeInfo *codeInfo;
							/*Clone*/
							Function *newFunction = CloneFunction(&F, Vmap, codeInfo);
							//CallInst* newCI = CallInst::Create(newFunction, 

							/*Replace*/
							//CI->removeFromParent();
							if(CI->getCalledFunction() == &F){
								CI->setCalledFunction(newFunction);
							}
							/*create a constant symbol of type Value*/
							
							llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(argC);
							if (constInt) {
								int intValue = constInt->getSExtValue();
							

								LLVMContext& context = llvm::getGlobalContext();
								ConstantInt* myConst = ConstantInt::get(Type::getInt32Ty(context), intValue, false);
							
								for (Function::arg_iterator arg = F.arg_begin(), end = F.arg_end(); arg != end; ++arg) {
									if (&*arg == argC) {
										argC->replaceAllUsesWith(myConst);
										errs() << myConst;
										errs() << "here";
									}
								}
							}
							//errs() << myConst;
						}
					}
				}
			}
		}
		//errs() << "Hello: " ;
		//errs().write_escaped(F.getName())<< "\n";
		//return false;
	}

};
}

char MyPass::ID = 0;
static RegisterPass<MyPass> X("hello", "Hello World Pass", false, false);



