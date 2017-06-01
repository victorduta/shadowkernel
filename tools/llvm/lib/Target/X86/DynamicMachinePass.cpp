#include <algorithm>
#include <cstdlib>

#include "X86.h"

#include "dlclass.hpp"
#include "DynamicMachinePass.h"

  using namespace llvm;

  FunctionPass* llvm::createX86DynamicMachinePass() {
    auto dlDynamicPass = new DLClass<DynamicMachinePass>(std::getenv("CODEGEN_PASS_PATH"));
    std::shared_ptr<DynamicMachinePass> dynamic_pass = dlDynamicPass->make_obj();
    return dynamic_pass->createX86DynamicMachinePass();
  }

