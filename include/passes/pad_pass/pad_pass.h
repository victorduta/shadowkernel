#ifndef PAD_PASS_H__
#define PAD_PASS_H__

#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetInstrInfo.h"

#define MIN_CALL_SIZE 2
#define PADDED_CALL_SIZE 8
#define CALL64_M_USED_OPERANDS 5

using namespace llvm;
namespace llvm
{
class PadCallInstruction: public MachineFunctionPass {
private:
    static char ID;
public:
    PadCallInstruction() : MachineFunctionPass(ID) {};

    virtual ~PadCallInstruction() {}

    virtual FunctionPass* createX86PadCallInstruction() = 0;

    virtual bool runOnMachineFunction(MachineFunction &MF) = 0;

    virtual MachineFunctionProperties getRequiredProperties() const = 0;
    virtual StringRef getPassName() const = 0;

    typedef PadCallInstruction * create_t();
    typedef void destroy_t(PadCallInstruction *);

};
    char PadCallInstruction::ID = 0;
}


#endif
