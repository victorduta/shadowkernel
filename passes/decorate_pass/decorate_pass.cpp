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

#include "decorate_pass.h"


namespace llvm {

    DecoratePass::DecoratePass() : ModulePass(ID) {}

    bool DecoratePass::runOnModule(Module &M)
    {

        doInit(M);
    	for (auto curFref = M.getFunctionList().begin(),
    	    	              endFref = M.getFunctionList().end();
    	    	              curFref != endFref; ++curFref)
        {
    	    		     Function *function = &(*curFref);
    	    		     runOnFunction(*function);

    	}
    	return true;
    }

    void DecoratePass::doInit(Module &M)
    {
    	Type *return_type = Type::getVoidTy(M.getContext());
    	vector<Type *> argument_type;

    	Type *arg1 =Type::getInt64Ty(M.getContext());

    	argument_type.push_back(arg1);

    	FunctionType* signature_type = FunctionType::get(return_type, argument_type, false);

    	Constant* c = M.getOrInsertFunction("setXXSignature", signature_type);

    	sigFunction = cast<Function>(c);

    	filter_str = M.getTypeByName("struct.from_entry");
    }
    void DecoratePass::runOnFunction(Function &F)
    {
    	vector<CallInst *> f_calls;

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
        				CallInst *callinstr =  dyn_cast<CallInst>(i);
        				Function *calee;

        				if (callinstr->isInlineAsm())
        				{
        					continue;
        				}
        				/* Direct calls are treated here */
        				if ((calee = callinstr->getCalledFunction()) != nullptr)
        				{
        					if (calee->isIntrinsic())
        					{
        						continue;
        					}
        					bool instrument = true;
        				    for(vector<string>::const_iterator fi= global_functions.begin(),
        				    	    		              fe = global_functions.end(); fi != fe; ++fi)
        				    {
        				        if (calee->getName().compare(*fi) == 0)
        				        {
        				           instrument = false;
        				           break;
        				        }
        				    }
        				    if(!instrument)
        				    	 continue;
        			     }
        				 else
        				 {
        					 if(callinstr->getNumArgOperands() >= 1)
        					 {
        						 Type *search = callinstr->getArgOperand(0)->getType();
        						 if (search->isPointerTy())
        						 {
        						     Type* ty = (static_cast<PointerType*>(search))->getElementType();
        						     /* Ommit calls that have from_entry* as first argument
        						      * These are calls to lbr_entry function and we don't need these to
        						      * be tracked
        						      */
        						     if (ty == filter_str)
        						     {
        						        continue;
        						     }

        						  }
        					 }
        				 }
        			     f_calls.push_back(callinstr);

        			}
        		}
        }

     	for(vector<CallInst *>::const_iterator fi= f_calls.begin(),
     	        			fe = f_calls.end(); fi != fe; ++fi)
     	{
     		CallInst *call = (*fi);
     		vector<Value *> args;
            args.push_back(ConstantInt::get(F.getParent()->getContext(), APInt(64, ++marker_generator)));
            CallInst::Create(sigFunction, args, "", call);
     	}
    }

    char DecoratePass::ID = 0;

    static RegisterPass<DecoratePass> X("lto_decorate", "LTO decoration pass",
                                 false ,
                                 true );
}
