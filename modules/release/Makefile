
# Run me with GNU make

ROOT=../..

include $(ROOT)/Makefile.inc

MCFLAGS += -fpad-calls
MODULE = shadow-module
OBJS = shadow.o


MODULE_NAME=$(MODULE).ko

#KERNEL_VERSION=$(shell uname -r)


KDIR ?= /lib/modules/$(KERNEL_VERSION)/build

MCFLAGS += -I$(INCLUDE_MODULE_DIR) -I$(INCLUDE_MODULE_DIR)/$(KERNEL_BUILD_TYPE) -I$(INCLUDE_WRAPPER_DIR)/$(KERNEL_BUILD_TYPE)


all: $(MODULE_NAME)

install:
	true

obj-m += $(MODULE).o
$(MODULE)-objs := $(OBJS)

$(MODULE_NAME): $(HEADERS) $(patsubst %.o,%.c,$(OBJS))
	$(QUIET) make -C $(KDIR) M=$(CURDIR) modules EXTRA_CFLAGS="$(MCFLAGS)" CC=$(LLVMBIN)/clang $(VERBOSE) LD=ld.gold



clean:
	$(RM) -rf *.o $(MODULE_NAME) .tmp_versions Module.symvers modules.order shadow-module.mod.c .*.o.cmd .*.ko.cmd 
