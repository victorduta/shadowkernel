
ROOT=..

include $(ROOT)/Makefile.inc


DIR=llvmLinux
WD=$(PWD)

ifeq ($(KERNEL_BUILD_TYPE),debug)
DFLAGS=-DLBR_DEBUG
endif

ifeq ($(KERNEL_BUILD_TYPE),release)
DFLAGS=-DLBR_RELEASE
endif

IFLAGS=-I$(INCLUDE_WRAPPER_DIR)/$(KERNEL_BUILD_TYPE) -I$(INCLUDE_COMMON_DIR)


KCFLAGS=-fpad-calls -include epilogue.h -Xclang -load -Xclang $(INSTALL_PASS_DIR)/lbr_pass.so $(DFLAGS) $(IFLAGS)
KCFLAGS_EXTRA=-DSKIP_INSTRUMENTATION
KCFLAGS_SCRIPTS=-DSKIP_SCRIPT_INSTRUMENTATION
CORES=-j8

PER_FILE_FLAGS += CFLAGS_devicetable-offsets.o+="$(KCFLAGS_SCRIPTS)" 
PER_FILE_FLAGS += CFLAGS_vclock_gettime.o+="$(KCFLAGS_EXTRA)"
PER_FILE_FLAGS += CFLAGS_empty.o+="$(KCFLAGS_SCRIPTS)"
PER_FILE_FLAGS += CFLAGS_bounds.o+="$(KCFLAGS_SCRIPTS)"

REDIRECT_BUILD_KERNEL= 1> kernel_dump 2> kernel_dump
REDIRECT_BUILD_MODULES= 1> kernel_module_dump 2> kernel_module_dump
REDIRECT_INSTALL_IMAGE= 1> kernel_install_dump 2> kernel_install_dump
REDIRECT_INSTALL_MODULES= 1> kernel_module_install_dump 2> kernel_module_install_dump


KERNEL_CONFIG_EXTRA=dbg_config

TARGETS= config build_kernel build_modules install_modules install_image install

all: $(TARGETS)

install:
	cd $(ROOT); ./copy_kernel.sh; cd $(WD)

install_image:
	sudo $(MAKE) -C $(DIR) install CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=ld.gold  KCFLAGS="$(KCFLAGS)" $(CORES) $(VERBOSE) $(REDIRECT_INSTALL_IMAGE)

install_modules:
	sudo $(MAKE) -C $(DIR) modules_install CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=ld.gold  KCFLAGS="$(KCFLAGS)" $(CORES) $(VERBOSE)  $(REDIRECT_INSTALL_MODULES)

build_modules:
	$(MAKE) -C $(DIR) modules CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=ld.gold  KCFLAGS="$(KCFLAGS)" $(CORES) $(VERBOSE) $(REDIRECT_BUILD_MODULES)

build_kernel:
	$(MAKE) -C $(DIR) CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=ld.gold ${PER_FILE_FLAGS}  KCFLAGS="$(KCFLAGS)" $(CORES) $(VERBOSE) $(REDIRECT_BUILD_KERNEL)

config: def_config $(KERNEL_CONFIG_EXTRA)

def_config: clean
	$(MAKE) -C $(DIR) x86_64_defconfig CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=ld.gold 
	$(MAKE) -C $(DIR) kvmconfig CC=$(CXX) HOSTCC=$(CXX) AS=$(AS)  LD=ld.gold 

dbg_config:
	cd $(DIR); ./scripts/kconfig/merge_config.sh .config .config-fragment; cd ..

clean: clean_kernel
	$(MAKE) -C $(DIR) mrproper

clean_kernel:
	cd $(ROOT);./clean_kernel.sh; cd $(WD)


