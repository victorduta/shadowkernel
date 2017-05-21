DIRS := null

ROOT=..

include $(ROOT)/Makefile.inc

ifeq ($(BUILD_LLVM_TINY),build_llvm_tiny)
LLVM_BUILD_SURCE=llvm_parallel
else
LLVM_BUILD_SURCE=llvm
endif


CONFIGFLAGS := -DLLVM_ENABLE_EH=ON -DLLVM_ENABLE_RTTI=ON 
ifeq ($(LLVM_BUILD_SURCE),llvm_parallel)
DIRS := llvm_parallel
CONFIGFLAGS += -DCMAKE_INSTALL_PREFIX=${LLVM_SOURCE}/bin_parallel
ALLDIRS += DIRS
else
CONFIGFLAGS += -DCMAKE_INSTALL_PREFIX=${LLVM_SOURCE}/bin
DIRS := llvm  
ALLDIRS += DIRS 
endif

ifeq (HAVE_GOLD,have_gold)
CONFIGFLAGS += -DLLVM_USE_LINKER=gold
endif

CONFIGFLAGS+= -DLLVM_TARGETS_TO_BUILD="X86"

ifeq ($(BUILD_LLVM_TINY),build_llvm_tiny)
CONFIGFLAGS+= -DLLVM_ENABLE_PROJECTS="clang;libcxx;libcxxabi;compiler-rt;polly"
endif


all: $(patsubst %,install_%,$(DIRS))

$(patsubst %,install_%,$(DIRS)): DIR = $(subst install_,build_,$@)
$(patsubst %,install_%,$(DIRS)):
	@echo Building LLVM $(DIR)...
	$(MAKE) -C $(DIR) install

clean:
	$(foreach DIR, $(DIRS), $(MAKE) -C $(DIR) distclean;)

distclean: clean

config: $(patsubst %,configure_%,$(DIRS))

$(patsubst %,configure_%,$(DIRS)): DIR = $(subst configure_,,$@)
$(patsubst %,configure_%,$(DIRS)):
	mkdir build_$(DIR) ; cd build_$(DIR) ; \
	cmake -g ¨Unix Makefiles¨ ${CONFIGFLAGS} ${LLVM_SOURCE}/${DIR}; \
	cd .. 


cleanconfig: $(patsubst %,cleanconfig_%,$(DIRS))

$(patsubst %,cleanconfig_%,$(DIRS)): DIR = $(subst cleanconfig_,,$@)
$(patsubst %,cleanconfig_%,$(DIRS)):
	rm -rf build_$(DIR)

install: all
