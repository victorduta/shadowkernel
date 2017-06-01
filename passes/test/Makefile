
ROOT=../..

include $(ROOT)/Makefile.inc
CFLAGS= -include epilogue.h -Xclang -load -Xclang $(INSTALL_PASS_DIR)/lbr_pass.so
OBJS=test_1.c

export CODEGEN_PASS_PATH=/home/victor/Disertation/Workplace/install/passes/pad_pass.so

KERNELARGS=  -g -D__KERNEL__ -Qunused-arguments -Wno-unknown-warning-option -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Wno-format-security -std=gnu89 -no-integrated-as -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -m64 -mno-80387 -mtune=generic -mno-red-zone -mcmodel=kernel -funit-at-a-time -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_FXSAVEQ=1 -DCONFIG_AS_SSSE3=1 -DCONFIG_AS_CRC32=1 -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1 -DCONFIG_AS_SHA1_NI=1 -DCONFIG_AS_SHA256_NI=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -O2 -Wframe-larger-than=2048 -fno-stack-protector -Wno-unused-variable -Wno-format-invalid-specifier -Wno-gnu -Wno-asm-operand-widths -Wno-initializer-overrides -fno-builtin -Wno-tautological-compare -mno-global-merge -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow  -Wno-initializer-overrides -Wno-unused-value -Wno-format -Wno-unknown-warning-option -Wno-sign-compare -Wno-format-zero-length -Wno-uninitialized -fno-stack-protector
all: clean test_1.plain

%.o: %.c
	$(CXX) $(CFLAGS) $(KERNELARGS) -fpad-calls -o $@ $<
%.plain: %.c
	$(CXX) -fpad-calls -c -fno-optimize-sibling-calls -O2  -o  $@ $<
%.ll: %.c
	$(CXX) $(CFLAGS) -S -c -emit-llvm -fno-optimize-sibling-calls -O2 -o  $@ $<
%.s: %.bc
	$(LLC) -print-machineinstrs=dead-mi-elimination -o $@ $<
	$(LLC) -help-hidden | grep isel
%.bc: %.c
	$(CXX) -emit-llvm -c $(KERNELARGS) -o $@ $<
	

help:
	$(OPT) --help-hidden | grep debug

clean:
	rm -f *.bc *.o *.s *.plain *.ll

