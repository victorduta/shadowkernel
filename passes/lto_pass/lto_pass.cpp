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
    	doInit();
    	for (auto curFref = M.getFunctionList().begin(),
    	              endFref = M.getFunctionList().end();
    	              curFref != endFref; ++curFref)
    	{
    		     Function *function = &(*curFref);
    		     runOnFunction(*function);

    	}
    	int i = 0;
    	int callsiteSize = directCalls.size();
    	outs() << "We have to inline " << callsiteSize << " callsites\n";
	    for(vector<CallInst*>::iterator it = directCalls.begin(),
	    		 et = directCalls.end(); it != et; ++it)
	    {
	    	 outs() << "Element " << ++i << "|"<< callsiteSize << " from function " << (*it)->getParent()->getParent()->getName() <<"\n";
	    	 (*it)->print(outs() << "\n");
	         InlineFunctionInfo info = InlineFunctionInfo(NULL);
	         InlineFunction(CallSite(*it), info);
	    }
    	return true;
    }

    void LtoPass::runOnFunction(Function &F)
    {
    	//F.print(outs() << "\n");

    	/* We to a DFS search. So mark the function as traced before we actually
    	 * do inlining of its content
    	 */
    	if (traced.count(F.getName()) > 0)
    	{
    		outs() << "We already tracked "+ F.getName() << "\n";
    		return;
    	}
    	if (F.isIntrinsic())
    	{
    		outs() << "Don't track llvm intrinsics " << "\n";
    		traced[F.getName()] = 1;
    		return;
    	}
    	traced[F.getName()] = 1;
    	tracedFunctions++;
    	outs() << "We will track "+ F.getName()+ " :" << tracedFunctions << "\n";
    	//outs() << "We must track function " + F.getName() << "\n";

    	for(Function::iterator f_begin= F.begin(),
    			 f_end = F.end(); f_begin != f_end; ++f_begin)
    	{
    		BasicBlock *block = &(*f_begin);

    		//block->print(outs() << "\n");

    		for(BasicBlock::iterator b_iterator = block->begin(),
    				b_end = block->end(); b_iterator != b_end; ++b_iterator)
    		{
    			if (isa<CallInst>(b_iterator))
    			{
    				Instruction *i = &(*b_iterator);
    				CallInst *callinstr =  dyn_cast<CallInst>(i);
    				Function *calee;
    				//callinstr.print(outs()<< "\n");

    				/* We have a direct call here, check if we track this call
    				 * and in that case inline it.
    				 */
    				if ((calee = callinstr->getCalledFunction()) != nullptr)
    				{
                        if (tracked.count(calee->getName()) > 0)
                        {

                            if (traced.count(calee->getName()) != 0)
                            {
                                 runOnFunction(*calee);
                            }
                            directCalls.push_back(callinstr);
                            //callinstr->print(outs());
                            //outs() << "----- pushing callsite for inlining\n";

                        }
                        //callinstr->print(outs());
                        //outs() << "---- direct call\n";
    				}
    				else
    				{
    					//callinstr->print(outs());
    					//outs() << "---- indirect call\n";
    				}

    				/* TODO implement indirect call logic here*/
    			}
    		}
    	}
    }
    void LtoPass::doInit()
    {
    	for (vector<string>::const_iterator it = expensive_functions.begin(),
    			et = expensive_functions.end(); it != et; ++it)
    	{
                tracked[*(it)] = 1;
    	}
    }

    char LtoPass::ID = 0;

    static RegisterPass<LtoPass> X("lto_inline", "LTO inlining pass",
                                 false ,
                                 true );

}
