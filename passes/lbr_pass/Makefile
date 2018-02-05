# Makefile for the AOPify pass
ROOT=../..

include $(ROOT)/Makefile.inc

PASSNAME := lbr_pass
OBJS := lbr_pass.o

IFLAGS := -I$(INCLUDE_WRAPPER_DIR)/$(KERNEL_BUILD_TYPE)

ifeq ($(KERNEL_BUILD_TYPE),debug)
DFLAGS= -DLBR_DEBUG
endif

ifeq ($(KERNEL_BUILD_TYPE),release)
DFLAGS= -DLBR_RELEASE
endif

ifeq ($(CHECK_FOR_LOCAL_BUFFERS),True)
DFLAGS+= -DCHECK_FOR_LOCAL_BUFFERS
endif

ifeq ($(INCLUDE_EXTENDED_MEASUREMENTS),True)
DFLAGS+= -DINCLUDE_EXTENDED_MEASUREMENTS
endif


include ../Makefile.inc


SAVED_DIRS :=
SAVED_FILES := lbr_pass.cpp Makefile

include $(ROOT)/Makefile.bak
