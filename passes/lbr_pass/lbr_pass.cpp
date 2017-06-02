

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

#include "lbr_pass.h"


using namespace llvm;
using namespace std;

namespace llvm {

//======================================
// Constructors
//======================================

LbrPass::LbrPass() : FunctionPass(ID) {};

//======================================
// Public methods
//======================================

bool LbrPass::doInitialization(Module &M)
{

	/* We pad each and every module so get reference to pad_2nops
	 * and pad_5nops
	 */
#ifdef LBR_PADD_CALLS
	getPads(&M);
#endif

	/* Manually blacklist modules that cause kernel compilation
	 * crashes if instrumented. In this case also remove instrumentation
	 * internals as the loader might fail because of undefined symbols.
	 */
	for(vector<string>::const_iterator it = jump_modules.begin(),
			        et = jump_modules.end(); it != et; ++it)
	{
		if(M.getName().startswith((*it)))
		{
#ifdef LBR_DEBUG_INFO
			 errs() << "doInitialization:Module matching regex " << *it << " will not be instrumented\n";
#endif
			 cleanModuleInstrumentation(M);
			 isInstrumentable = false;
			 return true;
		}
	}

	/* Get a pointer to the epilogue function */
	getEpilogue(&M);


	return false;
}

bool LbrPass::runOnFunction(Function &F)
{

#ifdef LBR_DEBUG_INFO
#ifdef LBR_DEBUG_INFO_EXTENDED
    errs() << "Before instrumentation\n";
    F.print(errs());
    errs() << "========================\n";
#endif
#endif

	/* Instrumentation internals will not be instrumented */
    for(vector<string>::const_iterator fi= global_functions.begin(),
    		              fe = global_functions.end(); fi != fe; ++fi)
    {
       if (F.getName().compare(*fi) == 0)
       {
#ifdef LBR_DEBUG_INFO
    	  errs() << "runOnFunction:Internal function " + *fi + " will not be instrumented!\n";
#endif
  	      return false;
       }
    }


    /* Add padding no matter if the module or function is blacklisted */
#ifdef LBR_PADD_CALLS
    for(vector<string>::const_iterator fi= global_pads.begin(),
    		              fe = global_pads.end(); fi != fe; ++fi)
    {
       if (F.getName().compare(*fi) == 0)
       {
#ifdef LBR_DEBUG_INFO
    	  errs() << "runOnFunction:Internal function " + *fi + " will not be instrumented!";
#endif
  	      return false;
       }
    }

    vector<instruction_t *> call_list;
    getInstructionList<CallInst>(F, call_list);

#ifdef LBR_DEBUG_STATS
    printCallStats(call_list);
#endif

    instrumentCallInstructions(call_list);
#endif
   	/* Check if module is blacklisted. */
   	if (!isInstrumentable)
   	{
   		return false;
   	}

    /* Functions that are manually blacklisted will not be instrumented */
    for(vector<string>::const_iterator it = jump_functions.begin(),
    		ie = jump_functions.end(); it != ie; ++it)
    {
  	  if(F.getName().compare(*it) == 0)
  	  {
#ifdef LBR_DEBUG_INFO
  		  errs() << "runOnFunction:Skip function " << *it << "!\n";
#endif
  		  return false;
  	  }
    }

	Fp = &F;
	M = Fp->getParent();

#ifdef CHECK_FOR_LOCAL_BUFFERS
    if (!RequiresStackProtector())
    {
//#ifdef LBR_DEBUG_INFO
    	errs() << "runOnFunction:Function " << F.getName() << " has no local buffers\n";
//#endif
    	return false;
    }
    else
    {
    	errs() << "runOnFunction:Function " << F.getName() << " has local buffers\n";
    }
#endif

    /* Get a list of return instructions and their coresponding
     * debug location
     */
    vector<instruction_t *> instruction_list;
    getInstructionList<ReturnInst>(F, instruction_list);


    for(vector<instruction_t *>::const_iterator it = instruction_list.begin(),
    		                    ie = instruction_list.end(); it != ie; ++it)
    {
    	  Instruction *ret_ins = (*it)->inst;
    	  DebugLoc*dbg_loc = (*it)->dbg_loc;

    	  /* Create a call to llvm.frame.address(0) and insert it before the return instruction */
    	  vector<Value *> frame_args;
    	  frame_args.push_back(ConstantInt::get(M->getContext(), APInt(32, 0)));
    	  Value *frame_func = Intrinsic::getDeclaration(M, Intrinsic::frameaddress);

    	  CallInst* frame_address = CallInst::Create(frame_func, frame_args, "", ret_ins);

    	  if (frame_address == NULL)
    	  {
#ifdef LBR_DEBUG_ERROR
    		  errs() << "runOnFunction:llvm.frameaddress(0) error\n";
#endif
    		  return false;
    	  }

    	  /* Create a call to lbr_epilogue(i8 *) and insert it before the return instruction */

    	  vector<Value *> epilogue_args;
    	  epilogue_args.push_back(frame_address);
    	  CallInst* epilogue_address = CallInst::Create(epilogue, epilogue_args, "", ret_ins);

    	  if (epilogue_address == NULL)
    	  {
#ifdef LBR_DEBUG_ERROR
    		  errs() << "runOnFunction::lbr_epilogue(i8*) error \n";
#endif
    		  return false;
    	  }
    	  epilogue_address->setDebugLoc(*dbg_loc);



#ifdef LBR_INCLUDE_INLINE
    	  InlineFunctionInfo info = InlineFunctionInfo(NULL);
    	  InlineFunction(CallSite(epilogue_address), info);
#endif
    }


//#ifdef LBR_DEBUG_INFO
       errs() << "After instrumentation\n";
       F.print(errs());
       errs() << "========================\n";
//#endif

	return true;
}


//======================================
// Private methods
//======================================

/* Remove instrumentation internals */
void LbrPass::cleanModuleInstrumentation(Module &M)
{

	/* Iterate over each global variable name and remove its occurence */
    for(vector<string>::const_iterator gi = global_variables.begin(),
			             ge = global_variables.end(); gi != ge; ++gi)
    {

		GlobalVariable *glob = M.getGlobalVariable(*gi);

		if (glob != NULL)
		{
			/* Iterate over the users of the global variable and remove
			 * the use of the global variable
			 */
			for(llvm::Value::user_iterator gui = glob->user_begin(),
					   gue = glob->user_end(); gui != gue ; ++gui)
		    {

				gui->replaceUsesOfWith(glob, UndefValue::get(glob->getType()));

			}
		}

    }

    /* Iterate over each functions and remove their occurence */
    for(vector<string>::const_iterator fi = global_functions.begin(),
    		             fe = global_functions.end(); fi != fe; ++fi )
    {
          Function *f = M.getFunction(*fi);
	      if (f != NULL)
	      {

	      	for(llvm::Function::iterator bi = f->begin(), be = f->end(); bi != be; ++bi)
	      	{
	      		BasicBlock *block = &(*bi);

	      	    for(llvm::BasicBlock::iterator bb = block->begin(); bb != block->end(); ++bb)
	      	    {
	      			  Instruction *i = &(*bb);
	      			  if(CallInst* ci = dyn_cast<CallInst>(i))
	      			  {
	      			      for (unsigned int j=0; j < ci->getNumArgOperands(); j++)
	      			      {
	      			          Value *arg = ci->getArgOperand(j);
	      			          i->replaceUsesOfWith(arg, UndefValue::get(arg->getType()));
	      			      }

	      			      Value *used = ci->getCalledValue();
	      			      i->replaceUsesOfWith(used, UndefValue::get(used->getType()));
	      			  }
	      	     }
	      	 }
	         f->replaceAllUsesWith(UndefValue::get(f->getType()));
	         f->removeFromParent();
	      }
     }

}

/* Get a  pointer to the epilogue */
void LbrPass::getEpilogue(Module *M)
{
    Type *return_type = Type::getVoidTy(M->getContext());
    vector<Type *> argument_type;

    Type *arg1 = Type::getInt8PtrTy(M->getContext(), 0);

    argument_type.push_back(arg1);

    FunctionType* epilogue_type = FunctionType::get(return_type, argument_type, false);

    Constant* c = M->getOrInsertFunction("lbr_epilogue", epilogue_type);


    if ((epilogue = cast<Function>(c)) == NULL)
    {
#ifdef LBR_DEBUG_ERROR
        errs() << "Cannot get reference to epilogue function!\n";
#endif
        isInstrumentable = false;
    }
}

bool LbrPass::HasAddressTaken(const Instruction *AI) {
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

bool LbrPass::ContainsProtectableArray(Type *Ty, bool &IsLarge,
                                              bool Strong,
                                              bool InStruct) const {
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
    if (ContainsProtectableArray(*I, IsLarge, Strong, true)) {
      // If the element is a protectable array and is large (>= SSPBufferSize)
      // then we are done.  If the protectable array is not large, then
      // keep looking in case a subsequent element is a large array.
      if (IsLarge)
        return true;
      NeedsProtector = true;
    }

  return NeedsProtector;
}


bool LbrPass::RequiresStackProtector(){
  bool Strong = false;
  bool NeedsProtector = false;

  if (Fp->hasFnAttribute(Attribute::StackProtectReq)) {
    NeedsProtector = true;
    Strong = true;
  } else if (NeedsStrongProtector)
    Strong = true;
  else if (!Fp->hasFnAttribute(Attribute::StackProtect))
    return false;

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
        if (ContainsProtectableArray(AI->getAllocatedType(), IsLarge, Strong)) {
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

void LbrPass::getPads(Module *M)
{
	Type *return_type = Type::getVoidTy(M->getContext());
	FunctionType* pad_type = FunctionType::get(return_type, false);

	Constant *c_2nops =  M->getOrInsertFunction("pad_2nops", pad_type);
    if ((pad_2nops = cast<Function>(c_2nops)) == NULL)
    {
#ifdef LBR_DEBUG_ERROR
        errs() << "Cannot get reference to pad_2nops function!\n";
#endif
    }

	Constant *c_5nops =  M->getOrInsertFunction("pad_5nops", pad_type);
    if ((pad_5nops = cast<Function>(c_5nops)) == NULL)
    {
#ifdef LBR_DEBUG_ERROR
        errs() << "Cannot get reference to pad_2nops function!\n";
#endif
    }

}

void LbrPass::printCallStats(vector<instruction_t *>& instruction_list)
{
	errs() << "===============================================\n";

	for(vector<instruction_t *>::const_iterator it = instruction_list.begin(),
	    		                    ie = instruction_list.end(); it != ie; ++it)
	{
		CallInst *instruction = static_cast<CallInst *>((*it)->inst);
		errs() << "OpCode: " << instruction->getOpcode() << "\n";

		instruction->print(errs());
		errs() << "\n";

	    Function *f = instruction->getCalledFunction();
	    if (f == nullptr)
	    {
	    	errs() << "Called function: No function call\n";
	    }
	    else
	    {
	    	errs() << "Called function: " << f->getName() << "\n";
	    }
	    Instruction *next = instruction->getNextNode();
	    if (next == nullptr)
	    {
	    	errs() << "Called function: Last function in basick block\n";
	    }
	    else
	    {
	    	errs() << "Next instruction:";
	    	next->print(errs());
	    	errs() << "\n";
	    }


	}

	errs() << "===============================================\n";
}

void LbrPass::instrumentCallInstructions(vector<instruction_t *>& instruction_list)
{
	for(vector<instruction_t *>::const_iterator it = instruction_list.begin(),
		    		                    ie = instruction_list.end(); it != ie; ++it)
	{
        CallInst *instruction = static_cast<CallInst *>((*it)->inst);
        DebugLoc *dbg_loc = (*it)->dbg_loc;
        Function *called = instruction->getCalledFunction();
        Function *to_insert;

        if (called == nullptr)
        {
        	/* Indirect call, for the moment we don't know for sure
        	 * the size of an indirect call. Might be 2,3,4,6 or 7
        	 */
        	to_insert = pad_5nops;
        }
        else
        {
            to_insert = pad_2nops;
            if(called->isIntrinsic())
            {
            	/* Don't pad calls to intrinsics */
            	continue;
            }
        }
        /* Add pad before the next instruction */
        Instruction *next = instruction->getNextNode();
        CallInst* padding_call;

        if (next != nullptr)
        {
           padding_call = CallInst::Create(to_insert, "", next);
        }
        else
        {
           /* If no next instruction is available, just add padding at the end of
            * basic block.
            */
           padding_call = CallInst::Create(to_insert, "", instruction->getParent());
        }

        padding_call->setDebugLoc(*dbg_loc);

        /* Always inline the padding */
        InlineFunctionInfo info = InlineFunctionInfo(NULL);
        InlineFunction(CallSite(padding_call), info);

	}
}

template<class T>
void LbrPass::getInstructionList(Function &F, vector<instruction_t *> &instruction_vec)
{
	DebugLoc location;
    for(llvm::Function::iterator bi = F.begin(), be= F.end(); bi != be; ++bi)
    {
        BasicBlock *bb = &(*bi);

        for(llvm::BasicBlock::iterator ii = bb->begin(), ie = bb->end();
      		                              ii != ie; ++ii)
        {

      	  Instruction *i = &(*ii);
      	  const DebugLoc &loc = i->getDebugLoc();
      	  if(loc)
      	  {
               location = loc;
      	  }

      	  if (dyn_cast<T>(i) != NULL)
      	  {
      		  instruction_t *return_instruction = new instruction_t;
              return_instruction->inst = i;
              return_instruction->dbg_loc = new DebugLoc;
              *(return_instruction->dbg_loc) = location;
              instruction_vec.push_back(return_instruction);

      	  }
        }

     }
}
template void LbrPass::getInstructionList<ReturnInst>(Function &F, vector<instruction_t *> &instruction_vec);
template void LbrPass::getInstructionList<CallInst>(Function &F, vector<instruction_t *> &instruction_vec);

char LbrPass::ID = 0;

/* Register our pass */
static void registerLbrPass(const PassManagerBuilder & ,
                         legacy::PassManagerBase &PM) {
  PM.add(new LbrPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_VectorizerStart,
		  registerLbrPass);
}
