#include <algorithm>

#include "X86.h"
#include "X86InstrInfo.h"
#include "X86Subtarget.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetInstrInfo.h"


using namespace llvm;


namespace {

  struct PadCallInstructions : public MachineFunctionPass {
    static char ID;
    PadCallInstructions() : MachineFunctionPass(ID)
                   , STI(nullptr), TII(nullptr) {}

    bool runOnMachineFunction(MachineFunction &MF) override;

    MachineFunctionProperties getRequiredProperties() const override {
      return MachineFunctionProperties().set(
          MachineFunctionProperties::Property::NoVRegs);
    }

    StringRef getPassName() const override {
      return "X86 pad call instructions till size(call)+nops == 7 ";
    }

    const X86Subtarget *STI;
    const TargetInstrInfo *TII;
  };
  char PadCallInstructions::ID = 0;
}

  bool PadCallInstructions::runOnMachineFunction(MachineFunction &MF) {
	  errs() << "We were able to do this\n";
	  return false;
  }

  FunctionPass* llvm::createX86PadCallInstructions() {
    return new PadCallInstructions();
  }



