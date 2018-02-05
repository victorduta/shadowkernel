using namespace llvm;
using namespace std;

#include <vector>
vector<string> global_functions = { "lbr_epilogue", "getXXSignature", "setXXSignature" };
unsigned long long marker_generator = 0;

namespace llvm {
  class DecoratePass : public ModulePass {
  public:
	     static char ID;
	     DecoratePass();

         virtual bool runOnModule (Module &M);
  private:
         Function *sigFunction;
         Type *filter_str;
         void doInit(Module &M);
         void runOnFunction(Function &F);
  };


}
