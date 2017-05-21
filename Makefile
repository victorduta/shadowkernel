ROOT=.

include $(ROOT)/Makefile.inc

SCRIPTS := clean_kernel.sh cmake.sh compute_call_graph.sh copy_kernel.sh disassemble_kernel.sh fixup.sh global_exports.sh
SCRIPTS += kernel_make.sh make_rootfs.sh pass_make.sh run_qemu.sh strip_function.sh

all: backup

SAVED_DIRS :=  passes
SAVED_FILES := Makefile Makefile.inc Makefile.bak $(SCRIPTS) 

include $(ROOT)/Makefile.bak


