#ifndef X86PADCALLINSTRUCTION_H
#define X86PADCALLINSTRUCTION_H


#include "llvm/CodeGen/MachineFunctionPass.h"


using namespace llvm;
namespace llvm
{
class PadCallInstruction: public MachineFunctionPass {
private:
    static char ID;
public:
    PadCallInstruction(): MachineFunctionPass(ID){}

    virtual ~PadCallInstruction() {}

    virtual FunctionPass* createX86PadCallInstruction();

    typedef PadCallInstruction * create_t();
    typedef void destroy_t(PadCallInstruction *);

};
    char PadCallInstruction::ID = 0;
}

#endif
