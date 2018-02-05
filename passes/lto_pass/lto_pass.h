using namespace llvm;
using namespace std;

#include <map>
#include <vector>

vector<string> functions = { "getXXSignature", "setXXSignature"};


namespace llvm {
  class LtoPass : public ModulePass {
  public:
	     static char ID;
         LtoPass();
         virtual bool runOnModule (Module &M);
  };


}
