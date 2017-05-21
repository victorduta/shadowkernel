
# Run me with GNU make

MCFLAGS += -DSHADOW_DEBUG
MODULE = shadow-module
OBJS = shadow.o \
	   lbr.o 


MODULE_NAME=$(MODULE).ko

#USED_MODULES=$(shell uname -r)

USED_MODULES=4.4.17+

KDIR ?= /lib/modules/$(USED_MODULES)/build

MCFLAGS += -I$(shell pwd)/../../include -I$(shell pwd)/../../include/modules/debug -I$(shell pwd)/../../include/wrapper/debug

#CC=../llvm-bin/bin/clang
LD=ld.gold

obj-m += $(MODULE).o
$(MODULE)-objs := $(OBJS)

$(MODULE_NAME): $(HEADERS) $(patsubst %.o,%.c,$(OBJS))
	$(QUIET) make -C $(KDIR) M=$(CURDIR) modules EXTRA_CFLAGS="$(MCFLAGS)"

ioctl: try_ioctl.c
	$(CC)  $(MCFLAGS) try_ioctl.c -o try_ioctl
finish: finish_test.c
	$(CC)  $(MCFLAGS) finish_test.c -o finish_test

all: $(MODULE_NAME) ioctl finish

install:
	true

clean:
	$(RM) -rf *.o $(MODULE_NAME) .tmp_versions Module.symvers modules.order shadow-module.mod.c .*.o.cmd .*.ko.cmd try_ioctl finish_test
