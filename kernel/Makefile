
ROOT=..

include $(ROOT)/Makefile.inc


DIRS=llvmLinux baseKernel
TARGETS= config build_kernel build_modules install_modules install_image install
CONFIG_TARGETS= clean_kernel clean def_config
WD=$(PWD)

ifeq ($(KERNEL_BUILD_TYPE),debug)
DFLAGS=-DLBR_DEBUG
endif

ifeq ($(KERNEL_BUILD_TYPE),release)
DFLAGS=-DLBR_RELEASE
endif

ifeq ($(NOP_PAD_KERNEL),True)
llvmLinux_KAFLAGS+=-DNOP_PADDING
llvmLinux_KCFLAGS+=-fpad-calls
endif

ifeq ($(LBR_INSTRUMENT_KERNEL),True)
llvmLinux_KCFLAGS+=-include epilogue.h -Xclang -load -Xclang $(INSTALL_PASS_DIR)/lbr_pass.so
endif

ifeq ($(ADD_DEBUG_KERNEL),True)
CONFIG_TARGETS+=dbg_config
endif

IFLAGS=-I$(INCLUDE_WRAPPER_DIR)/$(KERNEL_BUILD_TYPE) -I$(INCLUDE_COMMON_DIR)

KCFLAGS_EXTRA=-DSKIP_INSTRUMENTATION
KCFLAGS_SCRIPTS=-DSKIP_SCRIPT_INSTRUMENTATION
CORES=-j8



llvmLinux_KCFLAGS+= $(DFLAGS) $(IFLAGS)
llvmLinux_KCFLAGS_MODULES=$(llvmLinux_KCFLAGS)
llvmLinux_KCFLAGS_MODULES+=-DSKIP_SCRIPT_INSTRUMENTATION


llvmLinux_PER_FILE_FLAGS += CFLAGS_devicetable-offsets.o+="$(KCFLAGS_SCRIPTS)" 
llvmLinux_PER_FILE_FLAGS += CFLAGS_vclock_gettime.o+="$(KCFLAGS_EXTRA)"
llvmLinux_PER_FILE_FLAGS += CFLAGS_empty.o+="$(KCFLAGS_SCRIPTS)"
llvmLinux_PER_FILE_FLAGS += CFLAGS_bounds.o+="$(KCFLAGS_SCRIPTS)"

llvmLinux_REDIRECT_BUILD_KERNEL= 1> kernel_dump 2> kernel_dump
llvmLinux_REDIRECT_BUILD_MODULES= 1> kernel_module_dump 2> kernel_module_dump
llvmLinux_REDIRECT_INSTALL_IMAGE= 1> kernel_install_dump 2> kernel_install_dump
llvmLinux_REDIRECT_INSTALL_MODULES= 1> kernel_module_install_dump 2> kernel_module_install_dump

baseKernel_REDIRECT_BUILD_KERNEL= 1> base_kernel_dump 2> base_kernel_dump
baseKernel_REDIRECT_BUILD_MODULES= 1> base_kernel_module_dump 2> base_kernel_module_dump
baseKernel_REDIRECT_INSTALL_IMAGE= 1> base_kernel_install_dump 2> base_kernel_install_dump
baseKernel_REDIRECT_INSTALL_MODULES= 1> base_kernel_module_install_dump 2> base_kernel_module_install_dump



all: $(patsubst %,all_%,$(DIRS))

all_baseKernel: $(patsubst %,%_baseKernel,$(TARGETS))

all_llvmLinux: $(patsubst %,%_llvmLinux,$(TARGETS))

install_llvmLinux:
	cd $(ROOT); ./copy_kernel.sh; cd $(WD)
install_baseKernel:
	true


$(patsubst %,install_image_%,$(DIRS)): DIR = $(subst install_image_,,$@)
$(patsubst %,install_image_%,$(DIRS)): KCFLAGS = $($(DIR)_KCFLAGS_MODULES)
$(patsubst %,install_image_%,$(DIRS)): REDIRECT_INSTALL_IMAGE = $($(DIR)_REDIRECT_INSTALL_IMAGE)
$(patsubst %,install_image_%,$(DIRS)):
	sudo $(MAKE) -C $(DIR) install CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=ld.gold  KCFLAGS="$(KCFLAGS)" $(CORES) $(VERBOSE) $(REDIRECT_INSTALL_IMAGE)

$(patsubst %,install_modules_%,$(DIRS)): DIR = $(subst install_modules_,,$@)
$(patsubst %,install_modules_%,$(DIRS)): KCFLAGS_MODULES = $($(DIR)_KCFLAGS_MODULES)
$(patsubst %,install_modules_%,$(DIRS)): REDIRECT_INSTALL_MODULES = $($(DIR)_REDIRECT_INSTALL_MODULES)
$(patsubst %,install_modules_%,$(DIRS)):
	sudo $(MAKE) -C $(DIR) modules_install CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=ld.gold  KCFLAGS="$(KCFLAGS_MODULES)" $(CORES) $(VERBOSE)  $(REDIRECT_INSTALL_MODULES)

$(patsubst %,build_modules_%,$(DIRS)): DIR = $(subst build_modules_,,$@)
$(patsubst %,build_modules_%,$(DIRS)): KCFLAGS_MODULES = $($(DIR)_KCFLAGS_MODULES)
$(patsubst %,build_modules_%,$(DIRS)): REDIRECT_BUILD_MODULES = $($(DIR)_REDIRECT_BUILD_MODULES)
$(patsubst %,build_modules_%,$(DIRS)):
	$(MAKE) -C $(DIR) modules CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=ld.gold  KCFLAGS="$(KCFLAGS_MODULES)" $(CORES) $(VERBOSE) $(REDIRECT_BUILD_MODULES)

$(patsubst %,build_kernel_%,$(DIRS)): DIR = $(subst build_kernel_,,$@)
$(patsubst %,build_kernel_%,$(DIRS)): KCFLAGS = $($(DIR)_KCFLAGS)
$(patsubst %,build_kernel_%,$(DIRS)): KAFLAGS = $($(DIR)_KAFLAGS)
$(patsubst %,build_kernel_%,$(DIRS)): PER_FILE_FLAGS = $($(DIR)_PER_FILE_FLAGS)
$(patsubst %,build_kernel_%,$(DIRS)): REDIRECT_BUILD_KERNEL = $($(DIR)_REDIRECT_BUILD_KERNEL)
$(patsubst %,build_kernel_%,$(DIRS)):
	$(MAKE) -C $(DIR) CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=ld.gold $(PER_FILE_FLAGS)  KCFLAGS="$(KCFLAGS)" KAFLAGS="$(KAFLAGS)" $(CORES) $(VERBOSE) $(REDIRECT_BUILD_KERNEL)



config_baseKernel: $(patsubst %,%_baseKernel,$(CONFIG_TARGETS))

config_llvmLinux: $(patsubst %,%_llvmLinux,$(CONFIG_TARGETS))


dbg_config_llvmLinux:
	cd llvmLinux; ./scripts/kconfig/merge_config.sh .config .config-fragment; cd ..

dbg_config_baseKernel:
	true

$(patsubst %,def_config_%,$(DIRS)): DIR = $(subst def_config_,,$@)
$(patsubst %,def_config_%,$(DIRS)):
	$(MAKE) -C $(DIR) x86_64_defconfig CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=ld.gold 
	$(MAKE) -C $(DIR) kvmconfig CC=$(CXX) HOSTCC=$(CXX) AS=$(AS)  LD=ld.gold 
	

$(patsubst %,clean_%,$(DIRS)): DIR = $(subst clean_,,$@)
$(patsubst %,clean_%,$(DIRS)):
	$(MAKE) -C $(DIR) mrproper

$(patsubst %,clean_kernel_%,$(DIRS)): DIR = $(subst clean_kernel_,,$@)
$(patsubst %,clean_kernel_%,$(DIRS)):
	cd $(ROOT);./clean_kernel.sh ${DIR}; cd $(WD)

clean_dump:
	rm -f kernel_*
	rm -f base_kernel_*



