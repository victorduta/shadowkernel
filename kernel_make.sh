#!/usr/bin/sh

. fixup.sh
cd llvmLinux/
make mrproper

make CC=/home/victor/Disertation/Workplace/tools/bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/tools/bin/bin/clang AS=/home/victor/Disertation/Workplace/tools/bin/bin/llvm-as x86_64_defconfig

make CC=/home/victor/Disertation/Workplace/tools/bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/tools/bin/bin/clang AS=/home/victor/Disertation/Workplace/tools/bin/bin/llvm-as kvmconfig

./scripts/kconfig/merge_config.sh .config .config-fragment

#make CC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang AS=/home/victor/Disertation/Workplace/llvm-bin/bin/llvm-as oldconfig

#make CC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang AS=/home/victor/Disertation/Workplace/llvm-bin/bin/llvm-as  -j8 V=1 &> kernel_dump


make CC=/home/victor/Disertation/Workplace/tools/bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/tools/bin/bin/clang AS=/home/victor/Disertation/Workplace/tools/bin/bin/llvm-as  KCFLAGS="-fpad-calls -I/home/victor/Disertation/Workplace/include/wrapper -include /home/victor/Disertation/Workplace/include/epilogue.h -Xclang -load -Xclang /home/victor/Disertation/Workplace/install/passes/lbr_pass.so" KAFLAGS="-fpad-calls" -j8 V=1 &> kernel_dump

#make CC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang AS=/home/victor/Disertation/Workplace/llvm-bin/bin/llvm-as   -j8 V=1 &> kernel_dump


make CC=/home/victor/Disertation/Workplace/tools/bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/tools/bin/bin/clang AS=/home/victor/Disertation/Workplace/tools/bin/bin/llvm-as KCFLAGS="-fpad-calls -I/home/victor/Disertation/Workplace/include/wrapper -include /home/victor/Disertation/Workplace/include/epilogue.h -Xclang -load -Xclang /home/victor/Disertation/Workplace/install/passes/lbr_pass.so" KAFLAGS="-fpad-calls" V=1  modules &> kernel_module_dump

sudo make CC=/home/victor/Disertation/Workplace/tools/bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/tools/bin/bin/clang AS=/home/victor/Disertation/Workplace/tools/bin/bin/llvm-as KCFLAGS="-fpad-calls -I/home/victor/Disertation/Workplace/include/wrapper -include /home/victor/Disertation/Workplace/include/epilogue.h -Xclang -load -Xclang /home/victor/Disertation/Workplace/install/passes/lbr_pass.so" KAFLAGS="-fpad-calls" V=1  modules_install &> kernel_module_install_dump

sudo make CC=/home/victor/Disertation/Workplace/tools/bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/tools/bin/bin/clang AS=/home/victor/Disertation/Workplace/tools/bin/bin/llvm-as KCFLAGS="-fpad-calls -I/home/victor/Disertation/Workplace/include/wrapper -include /home/victor/Disertation/Workplace/include/epilogue.h -Xclang -load -Xclang /home/victor/Disertation/Workplace/install/passes/lbr_pass.so" KAFLAGS="-fpad-calls" V=1  install &> kernel_install_dump


#make CC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang AS=/home/victor/Disertation/Workplace/llvm-bin/bin/llvm-as  modules

#make CC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang AS=/home/victor/Disertation/Workplace/llvm-bin/bin/llvm-as modules_install

#make CC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang AS=/home/victor/Disertation/Workplace/llvm-bin/bin/llvm-as install #INSTALL_PATH=


#make CC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang AS=/home/victor/Disertation/Workplace/llvm-bin/bin/llvm-as HOSTLD=/home/victor/Disertation/Workplace/llvm-bin/bin/ld.lld LD=/home/victor/Disertation/Workplace/llvm-bin/bin/ld.lld x86_64_defconfig

#make CC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang HOSTCC=/home/victor/Disertation/Workplace/llvm-bin/bin/clang AS=/home/victor/Disertation/Workplace/llvm-bin/bin/llvm-as HOSTLD=/home/victor/Disertation/Workplace/llvm-bin/bin/ld.lld LD=/home/victor/Disertation/Workplace/llvm-bin/bin/ld.lld LDS=/usr/bin/ld.gold LDFLAGS=" -m elf_x86_64 --relocatable -o " V=1


