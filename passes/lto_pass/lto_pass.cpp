#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Support/Casting.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Support/Casting.h"

#include "lto_pass.h"
#include <vector>

namespace llvm {

    LtoPass::LtoPass() : ModulePass(ID) {}

    bool LtoPass::runOnModule(Module &M)
    {
    	int i = 0;
    	for (auto iterator = functions.begin(),
             end_iterator = functions.end(); iterator != end_iterator; ++iterator)
        	{
    		      Function *F = M.getFunction(*iterator);
    		      if (F != nullptr)
    		      {
    		    	  i = 0;
    		    	  for(auto user: F->users())
    		    	  {
    		    		  CallInst* I;
    		    	      if((I = dynamic_cast<CallInst*>(user)) != nullptr)
    		    	      {
                              //I->print(outs());
    		    	    	  InlineFunctionInfo info = InlineFunctionInfo(NULL);
    		    	    	  if (!InlineFunction(CallSite(I), info))
    		    	    	  {
    		    	    		  outs() << "Error while inlining\n";
    		    	    	  }
    		    	    	  else
    		    	    	  {
    		    	    		 i++;
    		    	    	  }
    		    	      }
    		    	  }
    		    	  outs() << *iterator << "|" << i << "\n";
    		      }
    		      else
    		      {
    		    	  outs() << "Error 1:Cannot find function " << *iterator << " in module\n";
    		      }
        	}
    	return true;
    }

    char LtoPass::ID = 0;

    static RegisterPass<LtoPass> X("lto_inline", "LTO inlining pass",
                                 false ,
                                 true );

}
