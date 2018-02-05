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

namespace llvm {
  class LtoPass : public ModulePass {
  public:
	     static char ID;
         LtoPass();

         virtual bool runOnModule (Module &M);
  private:
         unsigned long tracedFunctions = 0;
         map<string, int> tracked;
         map<string, int> traced;
         vector<CallInst*> directCalls;


         void doInit();
         void runOnFunction(Function &F);
  };


}
