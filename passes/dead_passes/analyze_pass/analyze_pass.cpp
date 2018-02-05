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
    	int i;
    	doInit();
    	for (auto iterator = tracked.begin(),
    			  end_iterator = tracked.end(); iterator != end_iterator; ++iterator)
    	{
    		Function *F = M.getFunction(iterator->first);

    		if ( F != nullptr)
    		{   i = 0;
    			for(auto user: F->users())
    			{
    				if(auto I = dyn_cast<CallInst>(user))
    				{
    					//I->print(outs());
    					i++;
    				}
    			}
    			if (i == 0)
    			{
    				outs() << "Warning 1:Function " << F->getName() << " not called in module\n";
    			}
    			else
    			{
    				outs() << "Success:Function " << F->getName() << " called in module:" << i << " times\n";
    			}
    			if (RequiresStackProtector(F, &M))
    			{
    				outs() << "Function " << F->getName() << " requires stack protector\n";
    			}
    			else
    			{
    				outs() << "Function " << F->getName() << " does not require stack protector\n";
    			}
    		}
    		else
    		{
    			outs() << "Error 1:Cannot find function " << iterator->first << " in module\n";
    		}
    	}


    	return true;
    }
    bool AnalyzePass::HasAddressTaken(const Instruction *AI) {
      for (const User *U : AI->users()) {
        if (const StoreInst *SI = dyn_cast<StoreInst>(U)) {
          if (AI == SI->getValueOperand())
            return true;
        } else if (const PtrToIntInst *SI = dyn_cast<PtrToIntInst>(U)) {
          if (AI == SI->getOperand(0))
            return true;
        } else if (isa<CallInst>(U)) {
          return true;
        } else if (isa<InvokeInst>(U)) {
          return true;
        } else if (const SelectInst *SI = dyn_cast<SelectInst>(U)) {
          if (HasAddressTaken(SI))
            return true;
        } else if (const PHINode *PN = dyn_cast<PHINode>(U)) {
          // Keep track of what PHI nodes we have already visited to ensure
          // they are only visited once.
          if (VisitedPHIs.insert(PN).second)
            if (HasAddressTaken(PN))
              return true;
        } else if (const GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(U)) {
          if (HasAddressTaken(GEP))
            return true;
        } else if (const BitCastInst *BI = dyn_cast<BitCastInst>(U)) {
          if (HasAddressTaken(BI))
            return true;
        }
      }
      return false;
    }

    bool AnalyzePass::ContainsProtectableArray(Type *Ty, bool &IsLarge,
                                                  bool Strong,
                                                  bool InStruct, Module *M) const {
      if (!Ty)
        return false;
      if (ArrayType *AT = dyn_cast<ArrayType>(Ty)) {
        if (!AT->getElementType()->isIntegerTy(8)) {
          // If  we're inside of a structure, don't
          // add stack protectors unless the array is a character array.
          // However, in strong mode any array, regardless of type and size,
          // triggers a protector.
          if (!Strong && InStruct)
            return false;
        }

        // If an array has more than SSPBufferSize bytes of allocated space, then we
        // emit stack protectors.
        if (ProtectedBufferSize <= M->getDataLayout().getTypeAllocSize(AT)) {
          IsLarge = true;
          return true;
        }

        if (Strong)
          // Require a protector for all arrays in strong mode
          return true;
      }

      const StructType *ST = dyn_cast<StructType>(Ty);
      if (!ST)
        return false;

      bool NeedsProtector = false;
      for (StructType::element_iterator I = ST->element_begin(),
                                        E = ST->element_end();
           I != E; ++I)
        if (ContainsProtectableArray(*I, IsLarge, Strong, true, M)) {
          // If the element is a protectable array and is large (>= SSPBufferSize)
          // then we are done.  If the protectable array is not large, then
          // keep looking in case a subsequent element is a large array.
          if (IsLarge)
            return true;
          NeedsProtector = true;
        }

      return NeedsProtector;
    }


    bool AnalyzePass::RequiresStackProtector(Function *Fp, Module *M){
      bool Strong = true;
      bool NeedsProtector = false;

      if (NeedsStrongProtector)
         Strong = true;

      for (const BasicBlock &BB : *Fp) {
        for (const Instruction &I : BB) {
          if (const AllocaInst *AI = dyn_cast<AllocaInst>(&I)) {
            if (AI->isArrayAllocation()) {
              if (const auto *CI = dyn_cast<ConstantInt>(AI->getArraySize())) {
                if (CI->getLimitedValue(ProtectedBufferSize) >= ProtectedBufferSize) {
                  // A call to alloca with size >= ProtectedBufferSize requires
                  // stack protectors.
                  NeedsProtector = true;
                } else if (Strong) {
                  // Require protectors for all alloca calls in strong mode.
                  NeedsProtector = true;
                }
              } else {
                NeedsProtector = true;
              }
              continue;
            }

            bool IsLarge = false;
            if (ContainsProtectableArray(AI->getAllocatedType(), IsLarge, Strong, false, M)) {
              NeedsProtector = true;
              continue;
            }

            if (Strong && HasAddressTaken(AI)) {
              NeedsProtector = true;
            }
          }
        }
      }

      return NeedsProtector;
    }

    void AnalyzePass::runOnFunction(Function &F)
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
    void AnalyzePass::doInit()
    {
    	for (vector<string>::const_iterator it = expensive_functions.begin(),
    			et = expensive_functions.end(); it != et; ++it)
    	{
                tracked[*(it)] = 1;
    	}
    }

    char AnalyzePass::ID = 0;

    static RegisterPass<AnalyzePass> X("lto_analyze", "LTO analyzing pass",
                                 false ,
                                 true );

}
