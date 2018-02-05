#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Support/Casting.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Support/Casting.h"
#include "analyze_pass.h"
#include <vector>

namespace llvm {

    AnalyzePass::AnalyzePass() : ModulePass(ID) {}

    bool AnalyzePass::runOnModule(Module &M)
    {
        doInit(M);
        outs() << "real deal\n";
    	for (auto curFref = M.getFunctionList().begin(),
    	    	              endFref = M.getFunctionList().end();
    	    	              curFref != endFref; ++curFref)
        {
    	    		     Function *function = &(*curFref);
    	    		     runOnFunction(*function);

    	}
    	outs() << "Number of calls:" << nrCalls << "\n";
    	outs() << "Number of direct calls:" << nrDirectCalls << "\n";
    	outs() << "Number of indirect calls:" << nrInDirectCalls << "\n";
    	outs() << "Number of assembly calls:" << nrInlineAsm << "\n";
    	return false;
    }
    void AnalyzePass::runOnFunction(Function &F)
    {

    	if (F.isIntrinsic())
    	{
    		return;
    	}
    	for(vector<string>::const_iterator fi= global_functions.begin(),
    	    		              fe = global_functions.end(); fi != fe; ++fi)
    	{
    	       if (F.getName().compare(*fi) == 0)
    	       {
    	  	      return;
    	       }
    	}
    	for(Function::iterator f_begin= F.begin(),
            			 f_end = F.end(); f_begin != f_end; ++f_begin)
            {
            		BasicBlock *block = &(*f_begin);

            		for(BasicBlock::iterator b_iterator = block->begin(),
            				b_end = block->end(); b_iterator != b_end; ++b_iterator)
            		{
            			Instruction *i = &(*b_iterator);
            			if (isa<CallInst>(b_iterator))
            			{
            			      CallInst *callinstr =  static_cast<CallInst*>(i);
            			      Function *calee;
            			      if (callinstr->isInlineAsm())
            			      {
            			    	  nrInlineAsm++;
            			    	  continue;

            			      }
            			      if ((calee = callinstr->getCalledFunction()) != nullptr)
            			      {

            			             if (calee->isIntrinsic())
            			             {
            			            	   nrIntrinsics++;
            			              	   continue;
            			             }
            			             bool instrument = true;
            			             for(vector<string>::const_iterator fi= global_functions.begin(),
            			              				    	    		              fe = global_functions.end(); fi != fe; ++fi)
            			             {
            			              	if (calee->getName().compare(*fi) == 0)
            			              	{
            			              		  nrGlobals++;
            			                      instrument = false;
            			              	      break;
            			              	}
            			              }
            			              if(!instrument)
            			              	   continue;
            			              nrDirectCalls++;

            			       }
            			      else
            			      {
            			    	 if(callinstr->getNumArgOperands() >= 1)
            			    	 {
            			    		  //string cli;
            			    		  //raw_string_ostream s(cli);
            			    		  //callinstr->print(s);
            			    		  //outs() << "|"+ s.str();
            			    		  //outs() << "\n";

                                      Type *search = callinstr->getArgOperand(0)->getType();

            			    		  if (search->isPointerTy())
            			    		  {
            			    			  Type* ty = (static_cast<PointerType*>(search))->getElementType();
            			    			  if (ty == from_entry)
            			    			  {
            			    				  continue;
            			    			  }

            			    		  }

            			    	  }
            			    	  nrInDirectCalls++;
            			      }
            			      nrCalls++;

            			}
            		}

            }
    }
    void AnalyzePass::doInit(Module &M)
    {
          this->M = &M;
          from_entry = M.getTypeByName("struct.from_entry");
    }

    char AnalyzePass::ID = 0;

    static RegisterPass<AnalyzePass> X("lto_analyze", "LTO analyzing pass",
                                 false ,
                                 true );

}
