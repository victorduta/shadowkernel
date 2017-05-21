
HAVE_LLVM=1

##########################################
#  Project specific defines
##########################################

BUILD_LLVM_TINY=build_llvm_tiny
HAVE_GOLD=no_have_gold
# debug or release
KERNEL_BUILD_TYPE=release



#########################################
# Top DIR of the project
#########################################
PREFIX=/home/victor/Disertation/Workplace



#
# Note: you should move all main projects: 
# kernel, llvm toolchain, passes, 
# modules must have a similar entry
# around here
#

INCLUDE_TOP_DIR=${PREFIX}/include
INCLUDE_PASS_DIR=${INCLUDE_TOP_DIR}/passes
INCLUDE_WRAPPER_DIR=${INCLUDE_TOP_DIR}/wrapper
INCLUDE_COMMON_DIR=${INCLUDE_TOP_DIR}/common

INSTALL_TOP_DIR=${PREFIX}/install
INSTALL_PASS_DIR=${INSTALL_TOP_DIR}/passes


BACKUP_DIR=${PREFIX}/backup

ECHO=echo
QUIET=
VERBOSE=V=1


##################################################################################################
# Pass related configuration (Default)
#
LLVM_SOURCE=${PREFIX}/tools
LLVM_MODULES=${PREFIX}/lkm
LLVM_PASSES=${PREFIX}/passes

LLVMPREFIX=${LLVM_SOURCE}/bin
LLVMBIN=$(LLVMPREFIX)/bin
LLVMINC=$(LLVMPREFIX)/include
LLVMLIB=$(LLVMPREFIX)/lib

PASSDIR=${PREFIX}/passes
##################################################################################################
# Compiler specific (Default)
#

CXX=$(LLVMBIN)/clang
OPT=$(LLVMBIN)/opt
LLC=$(LLVMBIN)/llc
AS=$(LLVMBIN)/llvm-as
LLVMEXTRACT=$(LLVMPREFIX)/bin/opt





##################################################################################################
# Configuration modifiers. 
# 


ifeq ($(HAVE_LLVM),1)
LLVM_VERSION=$(shell $(LLVMPREFIX)/bin/llvm-config --version | cut -d"." -f1,2 | sed "s/[^0-9]//g")
else
LLVM_VERSION=0
endif




