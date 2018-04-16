
ROOT=../..

include $(ROOT)/Makefile.inc
include Makefile.passes

CFLAGS= -include epilogue.h -Xclang -load -Xclang $(INSTALL_PASS_DIR)/lbr_pass.so
OBJS=test_1.c

export CODEGEN_PASS_PATH=/home/victor/Disertation/Workplace/install/passes/pad_pass.so

KERNELARGS=  -g -D__KERNEL__ -Qunused-arguments -Wno-unknown-warning-option -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Wno-format-security -std=gnu89 -no-integrated-as -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -m64 -mno-80387 -mtune=generic -mno-red-zone -mcmodel=kernel -funit-at-a-time -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_FXSAVEQ=1 -DCONFIG_AS_SSSE3=1 -DCONFIG_AS_CRC32=1 -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1 -DCONFIG_AS_SHA1_NI=1 -DCONFIG_AS_SHA256_NI=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -O2 -Wframe-larger-than=2048 -fno-stack-protector -Wno-unused-variable -Wno-format-invalid-specifier -Wno-gnu -Wno-asm-operand-widths -Wno-initializer-overrides -fno-builtin -Wno-tautological-compare -mno-global-merge -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow  -Wno-initializer-overrides -Wno-unused-value -Wno-format -Wno-unknown-warning-option -Wno-sign-compare -Wno-format-zero-length -Wno-uninitialized -fno-stack-protector
# $(LD) -m elf_x86_64  -r -o test_4.o  -L/usr/lib/gcc/x86_64-linux-gnu/5.4.0 -L/usr/lib/gcc/x86_64-linux-gnu/5.4.0/../../../x86_64-linux-gnu -L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/5.4.0/../../..  -L/lib -L/usr/lib  -L/home/victor/Disertation/Workplace/tools/bin/bin/../lib -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=mcpu=x86-64 test_3.o /usr/lib/gcc/x86_64-linux-gnu/5.4.0/crtend.o /usr/lib/gcc/x86_64-linux-gnu/5.4.0/../../../x86_64-linux-gnu/crtn.o


TEST_AR=../../tools/bin/bin/llvm-ar 
TEST_RAN=../../tools/bin/bin/llvm-ranlib

symbol_list=dump_stack early_dynamic_pgts early_fixup_exception early_gdt_descr early_idt_handler_array early_level4_pgt early_make_pgtable early_pmd_flags early_printk empty_zero_page _end initial_code initial_gs init_level4_pgt init_per_cpu__gdt_page init_per_cpu__irq_stack_union init_thread_union level1_fixmap_pgt level2_fixmap_pgt level2_kernel_pgt level3_kernel_pgt phys_base __print_symbol secondary_startup_64 stack_start start_cpu0 startup_64 _text x86_64_start_kernel boot_command_line boot_params __bss_start __bss_stop clear_page cpu_tlbstate early_dynamic_pgts early_idt_handler_array early_level4_pgt early_make_pgtable early_pmd_flags idt_descr idt_table init_level4_pgt lbr_epilogue load_ucode_bsp __memcpy memset phys_base reserve_ebda_region start_kernel x86_64_start_kernel x86_64_start_reservations lbr_epilogue memblock_reserve reserve_ebda_region acpi_early_init acpi_subsystem_init __alloc_bootmem alloc_pages_current anon_vma_init async_synchronize_full __bdevname blk_lookup_devt block_class boot_command_line buffer_init build_all_zonelists cad_pid calibrate_delay calibrate_delay_is_known calibration_delay_done call_function_init cgroup_init cgroup_init_early check_bugs class_find_device complete console_init console_printk cpu_all_bits cpu_number cpuset_init cpuset_init_smp cpu_startup_entry crashk_res cred_init _ctype current_task decompress_method __delay delayacct_init devtmpfs_mount disk_get_part do_execve do_no_restart_syscall do_one_initcall do_utimes driver_init driver_probe_done early_boot_irqs_disabled early_irq_init efi efi_enter_virtual_mode efi_free_boot_services efi_late_init envp_init find_task_by_pid_ns flush_delayed_fput fork_init free_initmem free_initrd_mem get_filesystem_list getname_kernel get_option gfp_allowed_mask hrtimers_init idr_init_cache __initcall0_start __initcall1_start __initcall2_start __initcall3_start __initcall4_start __initcall5_start __initcall6_start __initcall7_start initcall_debug __initcall_end __initcall_start init_cred init_espfix_bsp init_files init_fs init_idle_bootup_task init_IRQ init_irq_proc init_mm init_nsproxy init_pid_ns init_ramfs_fs __initramfs_size __initramfs_start initrd_below_start_ok initrd_end initrd_load initrd_start init_rootfs init_struct_pid init_task init_thread_union init_timers init_user_ns init_uts_ns integrity_load_keys ioremap_huge_init jiffies kasprintf kernel_thread key_init kfree kill_litter_super __kmalloc kmem_cache_init kmem_cache_init_late kstrdup __ksymtab_init_task __ksymtab_init_uts_ns __ksymtab_loops_per_jiffy __ksymtab_name_to_dev_t __ksymtab_reset_devices __ksymtab_static_key_initialized __ksymtab_system_state kthreadd kthreadd_task ktime_get late_time_init lbr_epilogue linux_banner linux_proc_banner load_default_elevator_module load_default_modules loops_per_jiffy lpj_fine mark_rodata_ro mdp_major md_run_setup memblock_virt_alloc_try_nid memcmp memcpy mem_init memmove memset min_low_pfn mount_block_root mount_nodev mount_root msleep name_to_dev_t nfs_root_data node_states nsfs_init numa_default_policy numa_policy_init page_alloc_init page_writeback_init panic parameq parameqn param_ops_bool parse_args parse_early_options parse_early_param percpu_init_late __per_cpu_offset perf_event_init phys_base pidhash_init pidmap_init __preempt_count prepare_namespace preset_lpj printk printk_all_partitions proc_caches_init proc_root_init profile_init put_device put_page radix_tree_init ramfs_fill_super random_int_secret_init _raw_spin_lock rcu_init rcu_scheduler_starting rd_doload read_current_timer real_root_dev register_filesystem reset_devices ROOT_DEV root_mountflags root_task_group saved_command_line sched_clock_init sched_init sched_init_smp schedule_preempt_disabled security_init set_cpu_active set_cpu_online set_cpu_possible set_cpu_present set_cpus_allowed_ptr set_task_stack_end_magic setup_arch __setup_end setup_log_buf setup_max_cpus setup_nr_cpu_ids setup_per_cpu_areas setup_per_cpu_pageset __setup_start shmem_fill_super shmem_init signals_init simple_strtol simple_strtoul smpboot_thread_init smp_init smp_ops smp_setup_processor_id snprintf softirq_init sort_main_extable sprintf sscanf start_kernel __start___param static_key_initialized __stop___param strchr strcmp strcpy strlcat strlcpy strlen strncasecmp strncmp strsep strstr sys_access sys_chdir sys_chmod sys_chown sys_chroot sys_close sys_dup sys_fchmod sys_fchown sys_ftruncate sys_ioctl sys_lchown sys_link sys_mkdir sys_mknod sys_mount sys_newlstat sys_newstat sys_open sys_rmdir sys_symlink system_state sys_unlink sys_write taskstats_init_early thread_info_cache_init tick_init time_init timekeeping_init trap_init unregister_filesystem __usermodehelper_set_disable_depth utsns_operations vfs_caches_init vfs_caches_init_early vmalloc_init wait_for_completion wait_for_device_probe warn_slowpath_fmt
symbol_undefined=$(foreach sym,$(symbol_list),-u $(sym))

all: clean test_1.plain

%.o: %.c
	$(CXX) $(CFLAGS) -O2 -o $@ $<
%.plain: %.c
	$(CXX) -fpad-calls -c -fno-optimize-sibling-calls -O2  -o  $@ $<
%.ll: %.c
	$(CXX) $(CFLAGS) -S -c -emit-llvm -fno-optimize-sibling-calls -O2 -o  $@ $<
%.s: %.bc
	$(LLC)  -print-after-all -meabi=gnu -O2 -filetype=asm  -o $@ $< 
%.bc: %.c
	$(CXX) -emit-llvm -c $(KERNELARGS) -o $@ $<
	
help:
	$(OPT) --help-hidden | grep debug

help_llc:
	$(LLC) -help

lto:
	$(CXX) -flto -c a.c -o a.o
	$(CXX) -flto -c test_2.c -o main.o 
	$(CXX) -v -flto a.o main.o -o main

lto2:
	$(CXX) -flto -c a.c -o a.o
	$(CXX) -c  test_2.c  -o main.o 
	$(CXX) -flto a.o main.o -o main

nolto:
	$(CXX) -c a.c -o a.o
	$(CXX) -c -no-integrated-as -v test_2.c -o main.o 
	$(CXX) -v a.o main.o -o main

forcelto:
	$(CXX) -flto -c a.c -o a.o
	$(CXX) -c test_2.c -o main.o 
#	LLVMgold -help
	$(LD)  -v -z relro --hash-style=gnu --eh-frame-hdr -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 /usr/lib/gcc/x86_64-linux-gnu/5.4.0/../../../x86_64-linux-gnu/crt1.o /usr/lib/gcc/x86_64-linux-gnu/5.4.0/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/5.4.0/crtbegin.o -L/usr/lib/gcc/x86_64-linux-gnu/5.4.0 -L/usr/lib/gcc/x86_64-linux-gnu/5.4.0/../../../x86_64-linux-gnu -L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/5.4.0/../../.. -L/home/victor/Disertation/Workplace/tools/bin/bin/../lib -L/lib -L/usr/lib -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=emit-llvm  -lgcc --as-needed -lgcc_s --no-as-needed -lc -lgcc --as-needed -lgcc_s --no-as-needed /usr/lib/gcc/x86_64-linux-gnu/5.4.0/crtend.o /usr/lib/gcc/x86_64-linux-gnu/5.4.0/../../../x86_64-linux-gnu/crtn.o a.o main.o -o main

test_link_1:
	$(CXX) -flto -c a.c -o a.o
	$(CXX) -flto -c test_2.c -o main.o
	$(TEST_AR) rcsD test_3.o
	$(LD) -m elf_x86_64  -r -o test_link.o -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=emit-llvm a.o main.o test_3.o
	$(CXX) -flto  test_link.o -o run_link
test_link_2:
	$(CXX) -flto -c a.c -o a.o
	$(CXX) -c test_2.c -o main.o
	$(LD) -m elf_x86_64  -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=emit-llvm -r -o test_link.o a.o main.o

test_link_3:
	$(CXX) -flto -c a.c -o a.o
	$(CXX) -c test_2.c -o main.o
	$(LD) -m elf_x86_64  -r -o test_link.o -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so  a.o main.o
	$(CXX) -flto test_link.o -o run_link

ar-gold:
	$(CXX) -c dump.c -o dump.o
	rm -f test_3.o
	$(TEST_AR)  --plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so --plugin-opt=emit-llvm rcsD test_3.o
	#$(TEST_AR)  scr --plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so test_3.o dump.o
	$(LD) -m elf_x86_64  -r -u decompress_method -o test_4.o -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=emit-llvm  test_3.o 


asm_lto:
	$(CXX) -c -flto test_asm.s -o test_asm.x
	$(CXX)  test_asm.x -o test_asm

asm_lto2:
	#$(CXX) -flto -c a.c -o a.o
	gcc -S -c  a.c -o a.s
	$(CXX) -flto -c a.s -o a.o
	$(CXX) -flto -c test_2.c -o main.o
	$(TEST_AR) rcsD test_3.o
	$(LD) -m elf_x86_64  -r -o test_link.o -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=emit-llvm a.o main.o test_3.o
	$(CXX) -flto  test_link.o -o run_link

fcd_1:
	gcc -S test_asm.c -o test_asm.s
	$(CXX)  test_asm.s -c -no-integrated-as -o fcd.x	
	fcd  --partial -e 0000000000000000 fcd.x > fcd.c

fcd_2:	
	fcd vmlinux > vmlinux.c

emit_llvm:
	$(CXX) -S -c -emit-llvm -fno-optimize-sibling-calls -o  test_asm.o test_asm.c
emit_llvm_2:
	clang++ -emit-llvm -S -o  test_asm.o test_asm.s

emit_llvm_3:
	clang++ -emit-llvm -S -o  test_asm.o test_asm_2.S

emit_llvm_4:
	clang -c -o  test_asm.o test_asm_2.S
link_lto:
	$(CXX) -flto -c test_link_lto_a.c -o test_link_lto_a.o
	$(CXX) -flto -c test_link_lto_b.c -o test_link_lto_b.o
	$(CXX) -flto -c test_link_lto_c.c -o test_link_lto_c.o
	$(CXX) -flto test_link_lto_a.o test_link_lto_b.o test_link_lto_c.o -o run_link

link_lto2:
	$(CXX)  -c test_link_lto_a.c -o test_link_lto_a.o
	$(CXX)  -c test_link_lto_b.c -o test_link_lto_b.o
	$(CXX)  -c test_link_lto_c.c -o test_link_lto_c.o
	$(CXX)  test_link_lto_a.o test_link_lto_b.o test_link_lto_c.o -o run_link

link_lto3:
	$(CXX) -flto -c test_link_lto_a.c -o test_link_lto_a.o
	$(CXX)  -c test_link_lto_b.c -o test_link_lto_b.o
	$(CXX) -flto -c test_link_lto_c.c -o test_link_lto_c.o
	$(LD) -m elf_x86_64  -r -o test_link1.o -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=save-temps test_link_lto_a.o test_link_lto_b.o test_link_lto_c.o
	#$(OPT) --help-hidden
	$(OPT) -load=$(INSTALL_PASS_DIR)/lbr_pass.so  -hello  test_link1.o.0.5.precodegen.bc -o test_link2.o
	$(LD) -m elf_x86_64 -z muldefs -r -o  test_link3.o -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so    test_link2.o test_link1.o 
	$(CXX)  test_link3.o -o run_link
	
ar:
	$(CXX) -flto -c dump.c -o dump.o
	rm -f test_3.o
	$(TEST_AR) rcsD test_3.o
	$(TEST_AR) cr test_3.o dump.o
	ltrace $(LD)  -r -o test_4.o test_3.o

link_data:
	$(CXX) -flto -c link_data_1.c -o link_data_1.o
	$(CXX) -c link_data_main.c -o link_data_main.o
	$(LD) --stats --verbose --warn-common -m elf_x86_64 -z muldefs   -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -r -o  link_data.o   link_data_1.o link_data_main.o 
	$(CXX) link_data.o -o execute_data 

link_data_2:
	$(CXX) -flto -c link_data_1.c -o link_data_1.o
	$(CXX) -c link_data_main.c -o link_data_main_1.o
	#$(LD) --retain-symbols-file symbols -o link_data_main.o link_data_main_1.o
	$(LD)  --verbose --warn-common -m elf_x86_64 -z muldefs   -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=-print-before-all -plugin-opt=emit-llvm  -r -o  link_data.o   link_data_1.o link_data_main_1.o 
	$(CXX) link_data.o -o execute_data 

link_data_1:
	$(CXX) -c link_data_1.c -o link_data_1.o
	$(CXX) -c link_data_main.c -o link_data_main.o
	$(LD) -m elf_x86_64 -z muldefs  -r -o  link_data.o link_data_1.o link_data_main.o 
	$(CXX) link_data.o -o execute_data
 
vmlinux:
	$(LD) -m elf_x86_64 -z muldefs -r -o  vmlinux.final -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so    vmlinux.bc vmlinux.o

vmlinux_2:
	$(LD) -m elf_x86_64 -z muldefs   -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -r -o  vmlinux.final vmlinux.os vmlinux.o 
vmlinux_1: vmlinux.s
	$(AS) --64 -o vmlinux.os vmlinux.s

vmlinux_3:
	$(AR) rcs libvmlinux.a vmlinux.o	
	$(LD) -m elf_x86_64 -z muldefs  -L. -r -o vmlinux.final vmlinux.os -lvmlinux
vmlinux_4:
	$(AR) rcs libvmlinux.a vmlinux.o
	$(AR) rcs libvmlinuxos.a vmlinux.os	
	$(LD) -m elf_x86_64 -z muldefs  -L. -r -o vmlinux.final --start-group -lvmlinux	-lvmlinuxos --end-group
vmlinux_5:
	rm -f empty.o 
	$(AR) rcsD empty.o
	$(LD) -m elf_x86_64 $(symbol_undefined) -r -o start.o empty.o
	$(LD) -m elf_x86_64 -z muldefs  -L. -r -o vmlinux.final start.o --start-group -lvmlinuxos -lvmlinux	 --end-group
vmlinux_6:
	$(LLC) -fpad_calls -O2 -o vmlinux.s vmlinux.int_bc
	$(AS) --64 -o vmlinux.os vmlinux.s
	$(LD) -m elf_x86_64 -r -o vmlinux.final	vmlinux.os vmlinux.int --start-group lib.a libs.a --end-group
test_linker:
	$(LLVM_LINK) -help
	
llc:
	llvm-as < /dev/null | $(LLC)  -mattr=help
lto_test_main:
	#$(LD) -m elf_x86_64  -r -o test_link1.o -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=O2  -plugin-opt=emit-llvm -plugin-opt=-time-passes test_link_lto_a.o test_link_lto_b.o test_link_lto_c.o
	$(LD) -m elf_x86_64  -r -o test_link1.o -plugin /home/victor/Disertation/Workplace/tools/bin/bin/../lib/LLVMgold.so -plugin-opt=-help  -plugin-opt=emit-llvm -plugin-opt=-time-passes test_link_lto_a.o test_link_lto_b.o test_link_lto_c.o
clean:  clean_lto
	cp vmlinux.bc vmlinux.bc1
	cp vmlinux.o vmlinux.o1
	rm -f *.bc *.o *.s *.plain *.ll run_link test_link a.o main.o *.x
	cp vmlinux.bc1 vmlinux.bc
	cp vmlinux.o1 vmlinux.o
	rm -f vmlinux.o1 vmlinux.bc1

