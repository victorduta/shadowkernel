
#include <vector>

using namespace llvm;
using namespace std;

string epilogue_name = "lbr_epilogue";

//#define LBR_DEBUG_INFO
//#define LBR_DEBUG_ERROR
//#define LBR_DEBUG_STATS
#define LBR_INCLUDE_INLINE
//#define LBR_PADD_CALLS

#ifdef LBR_DEBUG_INFO
//#define LBR_DEBUG_INFO_EXTENDED
#endif




#if defined(LBR_RELEASE) || defined(LBR_DEBUG)
#include "exclude_functions.h"
vector<string> jump_modules =  {"arch/x86/entry/vdso/vdso32/vclock_gettime.c", "arch/x86/entry/vdso/vclock_gettime.c", "arch/x86/entry/vdso/vgetcpu.c"};
#endif

#ifdef LBR_DEBUG
vector<string> global_functions = {"lbr_epilogue"};
vector<string> global_variables = {"check_wrapper"};
#warning Building LBR DEBUG PASS
#endif

#ifdef LBR_RELEASE
vector<string> global_functions = {"lbr_epilogue"};
vector<string> global_variables = {};
#warning Building LBR RELEASE PASS
#endif

#if !defined(LBR_RELEASE) && !defined(LBR_DEBUG)
vector<string> jump_functions = {};
vector<string> jump_modules =  {};
vector<string> global_variables = {"test_global"};
vector<string> global_functions = {"lbr_epilogue"};
#warning Building TEST PASS
#endif

vector<string> global_pads={"pad_2nops", "pad_5nops"};

namespace llvm {

 struct instruction_t
 {
	 Instruction *inst;
	 DebugLoc *dbg_loc;
 };

 class LbrPass : public FunctionPass{
  public:
	 static char ID;

	 LbrPass();

	 virtual bool runOnFunction(Function &F);
	 virtual bool doInitialization(Module &M);

  private:

     bool isInstrumentable = true;
     // Pointer to the epilogue
     Function *epilogue = NULL;

     /* Pointer to padding functions in case we do it
        at llvm IR level */
     Function *pad_2nops = NULL;
     Function *pad_5nops = NULL;

	 void cleanModuleInstrumentation(Module &M);
	 void getEpilogue(Module *M);
	 void getPads(Module *M);
	 bool hasLocalBuffers(Function &F);

	 template<class T> void getInstructionList(Function &F, vector<instruction_t *>& instruction_vec);

     void printCallStats(vector<instruction_t *>& instruction_vec);
     void instrumentCallInstructions(vector<instruction_t *>& instruction_vec);

};
}
