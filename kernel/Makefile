# sanitizer coverage
ROOT=..

include $(ROOT)/Makefile.inc


DIRS=llvmLinux baseKernel
TARGETS= config build_kernel build_modules install_modules install_image install
CONFIG_TARGETS= clean_kernel clean def_config
WD=$(PWD)

ifeq ($(LTO_ENABLED),True)
#llvmLinux_KCFLAGS+=-flto
endif

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


ifeq ($(INCLUDE_RDMSR),True)
DFLAGS+=-DINCLUDE_RDMSR
endif

ifeq ($(INCLUDE_RA_REWRITE),True)
DFLAGS+=-DINCLUDE_RA_REWRITE
endif

ifeq ($(INCLUDE_MEASURE_INSTRUMENTATION),True)
DFLAGS+=-DINCLUDE_MEASURE_INSTRUMENTATION
endif

ifeq ($(INCLUDE_EXTENDED_MEASUREMENTS),True)
DFLAGS+=-DINCLUDE_EXTENDED_MEASUREMENTS
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

ifeq ($(LTO_ENABLED),True)

LTO_EXTRA_VMLINUX=LTO_EXTRA_VMLINUX="-plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=O0 -plugin-opt=emit-llvm"
LTO_EXTRA_C_FLAGS=LTO_EXTRA_C_FLAGS="-flto"
LTO_EXTRA_LD_FLAGS=LTO_EXTRA_LD_FLAGS="-plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=O0 -plugin-opt=emit-llvm"
LTO_EXTRA_VMLINUX_FINAL=LTO_EXTRA_VMLINUX_FINAL="-plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=-code-model=kernel -plugin-opt=O0"
LTO_EXTRA_LLC_FLAGS=LTO_EXTRA_LLC_FLAGS="-code-model=kernel  -disable-tail-calls  -disable-fp-elim" 

LTO_LD=LTO_LD=$(DEFAULT_LD)
LTO_DIS=LTO_DIS=$(LLVM_DIS)
LTO_AS=LTO_AS=$(LLVM_AS)
LTO_OPT=LTO_OPT=$(OPT)

ifeq ($(NOP_PAD_KERNEL),True)
LTO_EXTRA_LLC_FLAGS +=-pad-calls
endif

ifeq ($(ACTIVATE_LTO_INLINING),True)
LTO_INLINE=LTO_INLINE=True
LTO_EXTRA_OPT_INLINE_FLAGS=LTO_EXTRA_OPT_INLINE_FLAGS="-load=$(INSTALL_PASS_DIR)/inline_pass.so  -lto_inline"
LTO_FLAGS += $(LTO_INLINE)
LTO_FLAGS += $(LTO_EXTRA_OPT_INLINE_FLAGS)
LTO_FLAGS += LTO_EXTRA_OPT_REINSTRUMENT_FLAGS="-load=$(INSTALL_PASS_DIR)/reinstrument_lbr.so -lto_lbr_instrument"
endif


ifeq ($(ACTIVATE_LTO_DEBUG),True)
LTO_DEBUG=LTO_DEBUG=True
LTO_FLAGS += $(LTO_DEBUG)
endif

ifeq ($(INCLUDE_EXTENDED_MEASUREMENTS),True)
LTO_DECORATE=LTO_DECORATE=True
LTO_EXTRA_OPT_DECORATE_FLAGS=LTO_EXTRA_OPT_DECORATE_FLAGS="-load=$(INSTALL_PASS_DIR)/decorate_pass.so -lto_decorate"
LTO_EXTRA_OPT_MARKER_FLAGS=LTO_EXTRA_OPT_MARKER_FLAGS="-load=$(INSTALL_PASS_DIR)/lto_pass.so  -lto_inline" 
endif


#CONFIG_TARGETS+=lto_config

endif

#CONFIG_TARGETS+=lto_config

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
	sudo $(MAKE) -C $(DIR) install CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=$(LD)  KCFLAGS="$(KCFLAGS)" $(CORES) $(VERBOSE) $(REDIRECT_INSTALL_IMAGE)

$(patsubst %,install_modules_%,$(DIRS)): DIR = $(subst install_modules_,,$@)
$(patsubst %,install_modules_%,$(DIRS)): KCFLAGS_MODULES = $($(DIR)_KCFLAGS_MODULES)
$(patsubst %,install_modules_%,$(DIRS)): REDIRECT_INSTALL_MODULES = $($(DIR)_REDIRECT_INSTALL_MODULES)
$(patsubst %,install_modules_%,$(DIRS)):
	sudo $(MAKE) -C $(DIR) modules_install CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=$(LD)  KCFLAGS="$(KCFLAGS_MODULES)" $(CORES) $(VERBOSE)  $(REDIRECT_INSTALL_MODULES)

$(patsubst %,build_modules_%,$(DIRS)): DIR = $(subst build_modules_,,$@)
$(patsubst %,build_modules_%,$(DIRS)): KCFLAGS_MODULES = $($(DIR)_KCFLAGS_MODULES)
$(patsubst %,build_modules_%,$(DIRS)): REDIRECT_BUILD_MODULES = $($(DIR)_REDIRECT_BUILD_MODULES)
$(patsubst %,build_modules_%,$(DIRS)):
	$(MAKE) -C $(DIR) modules CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=$(LD)  KCFLAGS="$(KCFLAGS_MODULES)" $(CORES) $(VERBOSE) $(REDIRECT_BUILD_MODULES)

$(patsubst %,build_kernel_%,$(DIRS)): DIR = $(subst build_kernel_,,$@)
$(patsubst %,build_kernel_%,$(DIRS)): KCFLAGS = $($(DIR)_KCFLAGS)
$(patsubst %,build_kernel_%,$(DIRS)): KAFLAGS = $($(DIR)_KAFLAGS)
$(patsubst %,build_kernel_%,$(DIRS)): KLDFLAGS = $($(DIR)_KLDFLAGS)
$(patsubst %,build_kernel_%,$(DIRS)): PER_FILE_FLAGS = $($(DIR)_PER_FILE_FLAGS)
$(patsubst %,build_kernel_%,$(DIRS)): REDIRECT_BUILD_KERNEL = $($(DIR)_REDIRECT_BUILD_KERNEL)
$(patsubst %,build_kernel_%,$(DIRS)):
	$(MAKE) -C $(DIR) LLVM_NM=$(LLVM_NM) LLC=$(LLC) CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=$(LD) HAVE_LTO=$(LTO_ENABLED) $(LTO_LD) $(LTO_DIS) $(LTO_AS) $(LTO_OPT) $(LTO_EXTRA_VMLINUX) $(LTO_EXTRA_VMLINUX_FINAL) $(LTO_EXTRA_C_FLAGS) $(LTO_EXTRA_LD_FLAGS) $(LTO_EXTRA_LLC_FLAGS) $(LTO_EXTRA_OPT_FLAGS) $(LTO_EXTRA_OPT_DECORATE_FLAGS) $(LTO_EXTRA_OPT_MARKER_FLAGS)  $(LTO_DECORATE) $(LTO_FLAGS) $(PER_FILE_FLAGS) KCFLAGS="$(KCFLAGS)" KAFLAGS="$(KAFLAGS)" $(CORES) $(VERBOSE) $(REDIRECT_BUILD_KERNEL)
	true



config_baseKernel: $(patsubst %,%_baseKernel,$(CONFIG_TARGETS))

config_llvmLinux: $(patsubst %,%_llvmLinux,$(CONFIG_TARGETS))


lto_config_llvmLinux:
	cd llvmLinux; ./scripts/kconfig/merge_config.sh .config .config-lto; cd ..
lto_config_baseKernel:
	true;

dbg_config_llvmLinux:
	cd llvmLinux; ./scripts/kconfig/merge_config.sh .config .config-fragment; cd ..

dbg_config_baseKernel:
	true

$(patsubst %,def_config_%,$(DIRS)): DIR = $(subst def_config_,,$@)
$(patsubst %,def_config_%,$(DIRS)):
	$(MAKE) -C $(DIR) x86_64_defconfig CC=$(CXX) HOSTCC=$(CXX) AS=$(AS) LD=$(LD) 
	$(MAKE) -C $(DIR) kvmconfig CC=$(CXX) HOSTCC=$(CXX) AS=$(AS)  LD=$(LD) 
	

$(patsubst %,clean_%,$(DIRS)): DIR = $(subst clean_,,$@)
$(patsubst %,clean_%,$(DIRS)):
	$(MAKE) -C $(DIR) mrproper

$(patsubst %,clean_kernel_%,$(DIRS)): DIR = $(subst clean_kernel_,,$@)
$(patsubst %,clean_kernel_%,$(DIRS)):
	cd $(ROOT);./clean_kernel.sh ${DIR}; cd $(WD)

clean_dump:
	rm -f kernel_*
	rm -f base_kernel_*




