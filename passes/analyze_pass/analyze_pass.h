using namespace llvm;
using namespace std;
#include <map>
#include <vector>
vector<string> global_functions = { "lbr_epilogue", "getXXSignature", "setXXSignature" };
namespace llvm {
  class AnalyzePass : public ModulePass {
  public:
	     static char ID;
	     AnalyzePass();

         virtual bool runOnModule (Module &M);
  private:
         unsigned long long nrInlineAsm = 0;
         unsigned long long nrDirectCalls = 0;
         unsigned long long nrEntryCalls = 0;
         unsigned long long nrInDirectCalls = 0;
         unsigned long long nrIntrinsics = 0;
         unsigned long long nrCalls = 0;
         unsigned long long nrGlobals = 0;
         Module *M;
         Type *from_entry;
         void doInit(Module &M);
         void runOnFunction(Function &F);
  };


}
