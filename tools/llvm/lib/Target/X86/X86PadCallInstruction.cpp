#include <algorithm>

#include "X86.h"

#include "dlclass.hpp"
#include "X86PadCallInstruction.h"

  using namespace llvm;

  FunctionPass* llvm::createX86PadCallInstruction() {
    auto dlDynamicPass = new DLClass<PadCallInstruction>("/home/victor/Disertation/Workplace/install/passes/pad_pass.so");
    std::shared_ptr<PadCallInstruction> dynamic_pass = dlDynamicPass->make_obj();
    return dynamic_pass->createX86PadCallInstruction();
  }

