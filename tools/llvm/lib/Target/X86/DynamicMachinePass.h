#ifndef X86PADCALLINSTRUCTION_H
#define X86PADCALLINSTRUCTION_H


#include "llvm/CodeGen/MachineFunctionPass.h"


using namespace llvm;
namespace llvm
{
class DynamicMachinePass: public MachineFunctionPass {
private:
    static char ID;
public:
    DynamicMachinePass(): MachineFunctionPass(ID){}

    virtual ~DynamicMachinePass() {}

    virtual FunctionPass* createX86DynamicMachinePass();

    typedef DynamicMachinePass* create_t();
    typedef void destroy_t(DynamicMachinePass *);

};
    char DynamicMachinePass::ID = 0;
}

#endif
