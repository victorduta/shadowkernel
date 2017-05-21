#!/bin/sh
SOURCE_TREE=llvm_source_tree_4.0
rm -rf /home/victor/Disertation/Workplace/${SOURCE_TREE}/llvm-build
mkdir /home/victor/Disertation/Workplace/${SOURCE_TREE}/llvm-build
cd /home/victor/Disertation/Workplace/${SOURCE_TREE}/llvm-build
cmake -g ¨Unix Makefiles¨ -DLLVM_ENABLE_EH=ON -DLLVM_ENABLE_RTTI=ON -DCMAKE_INSTALL_PREFIX=/home/victor/Disertation/Workplace/llvm-bin /home/victor/Disertation/Workplace/${SOURCE_TREE}/llvm 
make
make install
#echo /home/victor/Disertation/Workplace/${SOURCE_TREE}/llvm-build
