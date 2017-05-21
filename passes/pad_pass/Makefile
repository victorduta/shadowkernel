# Makefile for the AOPify pass
ROOT=../..

include $(ROOT)/Makefile.inc

PASSNAME := pad_pass
OBJS := pad_pass.o
IFLAGS := -I$(LLVM_SOURCE)/llvm_parallel/lib/Target/X86 -I$(LLVM_SOURCE)/build_llvm_parallel/lib/Target/X86
LIBS :=
include ../Makefile.inc


SAVED_DIRS :=
SAVED_FILES := pad_pass.cpp Makefile

include $(ROOT)/Makefile.bak
