#!/bin/bash

PATH_TO_BACKUP=/home/victor/Disertation/Workplace/tools/llvm
# fixups are hardcoded for the moment X86PadCallInstruction.h
#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/X86MCInstLower.cpp /home/victor/Disertation/Workplace/tools/llvm_parallel/lib/Target/X86/X86MCInstLower.cpp
#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/X86AsmPrinter.cpp /home/victor/Disertation/Workplace/tools/llvm_parallel/lib/Target/X86/X86AsmPrinter.cpp
#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/X86AsmPrinter.h /home/victor/Disertation/Workplace/tools/llvm_parallel/lib/Target/X86/X86AsmPrinter.h

#

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/dlclass.cpp /home/victor/Disertation/Workplace/tools/llvm/lib/Target/X86/dlclass.cpp

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/dlclass.hpp /home/victor/Disertation/Workplace/tools/llvm/lib/Target/X86/dlclass.hpp


cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/DynamicMachinePass.h /home/victor/Disertation/Workplace/tools/llvm/lib/Target/X86/DynamicMachinePass.h

cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/DynamicMachinePass.cpp /home/victor/Disertation/Workplace/tools/llvm/lib/Target/X86/DynamicMachinePass.cpp

cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/X86.h /home/victor/Disertation/Workplace/tools/llvm/lib/Target/X86/X86.h

cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/X86TargetMachine.cpp /home/victor/Disertation/Workplace/tools/llvm/lib/Target/X86/X86TargetMachine.cpp

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/include/llvm/Support/CodeGen.h /home/victor/Disertation/Workplace/tools/llvm/include/llvm/Support/CodeGen.h

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/include/llvm/Target/TargetMachine.h /home/victor/Disertation/Workplace/tools/llvm/include/llvm/Target/TargetMachine.h

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/TargetMachine.cpp  /home/victor/Disertation/Workplace/tools/llvm/lib/Target/TargetMachine.cpp

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/include/llvm/Target/TargetOptions.h  /home/victor/Disertation/Workplace/tools/llvm/include/llvm/Target/TargetOptions.h

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/tools/clang/lib/CodeGen/BackendUtil.cpp /home/victor/Disertation/Workplace/tools/llvm/tools/clang/lib/CodeGen/BackendUtil.cpp

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/tools/clang/include/clang/Frontend/CodeGenOptions.def /home/victor/Disertation/Workplace/tools/llvm/tools/clang/include/clang/Frontend/CodeGenOptions.def

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/tools/clang/lib/Frontend/CompilerInvocation.cpp /home/victor/Disertation/Workplace/tools/llvm/tools/clang/lib/Frontend/CompilerInvocation.cpp
 
#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/tools/clang/include/clang/Driver/Options.td /home/victor/Disertation/Workplace/tools/llvm/tools/clang/include/clang/Driver/Options.td
#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/tools/clang/lib/Driver/Tools.cpp /home/victor/Disertation/Workplace/tools/llvm/tools/clang/lib/Driver/Tools.cpp




#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/X86PadCallInstruction.h ${PATH_TO_BACKUP}/X86PadCallInstruction.h

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/dlclass.cpp ${PATH_TO_BACKUP}/dlclass.cpp

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/dlclass.hpp  ${PATH_TO_BACKUP}/dlclass.hpp

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/X86PadCallInstruction.cpp ${PATH_TO_BACKUP}/X86PadCallInstruction.cpp


#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/X86PadCallInstructions.cpp  ${PATH_TO_BACKUP}/X86PadCallInstructions.cpp

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/X86.h  ${PATH_TO_BACKUP}/X86.h

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/X86/X86TargetMachine.cpp ${PATH_TO_BACKUP}/X86TargetMachine.cpp

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/include/llvm/Support/CodeGen.h  ${PATH_TO_BACKUP}/CodeGen.h

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/include/llvm/Target/TargetMachine.h  ${PATH_TO_BACKUP}/TargetMachine.h

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/lib/Target/TargetMachine.cpp  ${PATH_TO_BACKUP}/TargetMachine.cpp

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/include/llvm/Target/TargetOptions.h   ${PATH_TO_BACKUP}/TargetOptions.h

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/tools/clang/lib/CodeGen/BackendUtil.cpp  ${PATH_TO_BACKUP}/BackendUtil.cpp

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/tools/clang/include/clang/Frontend/CodeGenOptions.def  ${PATH_TO_BACKUP}/CodeGenOptions.def

#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/tools/clang/lib/Frontend/CompilerInvocation.cpp  ${PATH_TO_BACKUP}/CompilerInvocation.cpp
 
#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/tools/clang/include/clang/Driver/Options.td  ${PATH_TO_BACKUP}/Options.td
#cp /home/victor/workspace/pass-maker/passes/llvm_source_tree_4.0/llvm/tools/clang/lib/Driver/Tools.cpp  ${PATH_TO_BACKUP}/Tools.cpp
