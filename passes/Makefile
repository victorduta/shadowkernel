DIRS := lbr_pass pad_pass

ROOT=..
include $(ROOT)/Makefile.inc

all: $(patsubst %,build_%,$(DIRS))

$(patsubst %,build_%,$(DIRS)): DIR = $(subst build_,,$@)
$(patsubst %,build_%,$(DIRS)):
	@echo Building LLVM $(DIR)...
	$(MAKE) -C $(DIR) install

clean:
	$(foreach DIR, $(DIRS), $(MAKE) -C $(DIR) distclean;)

distclean: clean

install: all


SAVED_DIRS :=  pad_pass lbr_pass
SAVED_FILES := Makefile Makefile.inc

include $(ROOT)/Makefile.bak
