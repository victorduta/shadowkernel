using namespace llvm;
using namespace std;

#include <map>
#include <vector>
//#define DEBUG_LTO

#ifdef DEBUG_LTO
vector<string> expensive_functions = {"sum", "prod", "do_some_ops", "do_some_other_ops", "cumpulative_ops"};
#else
#include "expensive_functions.h"
#endif

unsigned int ProtectedBufferSize = 1;
bool NeedsStrongProtector = true;

namespace llvm {
  class AnalyzePass : public ModulePass {
  public:
	     static char ID;
	     AnalyzePass();

         virtual bool runOnModule (Module &M);
  private:
         bool HasAddressTaken(const Instruction *AI);
         bool ContainsProtectableArray(Type *Ty, bool &IsLarge,
                 bool Strong,
                 bool InStruct, Module *M) const;
         bool RequiresStackProtector(Function *Fp, Module *M);
         SmallPtrSet<const PHINode *, 16> VisitedPHIs;

         unsigned long tracedFunctions = 0;
         map<string, int> tracked;
         map<string, int> traced;
         vector<CallInst*> directCalls;


         void doInit();
         void runOnFunction(Function &F);
  };


}
