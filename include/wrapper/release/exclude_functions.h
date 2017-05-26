vector<string> jump_functions = {
"verify_cpu",
"syscall_trace_enter_phase1",
"syscall_trace_enter_phase2",
"prepare_exit_to_usermode",
"syscall_return_slowpath",
"do_syscall_32_irqs_off",
"do_fast_syscall_32",
"__switch_to",
"sys_rt_sigreturn",
"do_divide_error",
"do_overflow",
"do_invalid_op",
"do_coprocessor_segment_overrun",
"do_invalid_TSS",
"do_segment_not_present",
"do_stack_segment",
"do_alignment_check",
"do_double_fault",
"do_bounds",
"do_general_protection",
"do_int3",
"sync_regs",
"fixup_bad_iret",
"do_debug",
"do_coprocessor_error",
"do_simd_coprocessor_error",
"do_spurious_interrupt_bug",
"do_device_not_available",
"do_IRQ",
"smp_x86_platform_ipi",
"smp_kvm_posted_intr_ipi",
"smp_kvm_posted_intr_wakeup_ipi",
"do_nmi",
"smp_irq_work_interrupt",
"smp_deferred_error_interrupt",
"smp_threshold_interrupt",
"smp_thermal_interrupt",
"x86_acpi_enter_sleep_state",
"smp_reboot_interrupt",
"smp_reschedule_interrupt",
"smp_call_function_interrupt",
"smp_call_function_single_interrupt",
"smp_apic_timer_interrupt",
"smp_spurious_interrupt",
"smp_error_interrupt",
"smp_irq_move_cleanup_interrupt",
"swap_pages",
"trampoline_handler",
"do_page_fault",
"paranoid_entry",
"error_entry",
"__do_softirq",
"schedule_tail",
"rwsem_wake",
"rwsem_downgrade_wake",
"swsusp_save",
"early_printk",
"__print_symbol",
"__mutex_lock_slowpath",
"__mutex_unlock_slowpath",
"rwsem_down_read_failed",
"rwsem_down_write_failed",
"SyS_execve",
"sys_execve",
"SyS_execveat",
"sys_execveat",
"dump_stack",
"this_cpu_cmpxchg16b_emu",
"copy_user_generic_unrolled",
"copy_user_generic_string",
"copy_user_enhanced_fast_string",
"__get_user_1",
"__get_user_2",
"__get_user_4",
"__get_user_8",
"__put_user_2",
"__put_user_4",
"__put_user_8",
"call_rwsem_down_read_failed",
"call_rwsem_down_write_failed",
"call_rwsem_wake",
"call_rwsem_downgrade_wake",
"__sw_hweight32",
"__sw_hweight64",
"save_processor_state",
"early_make_pgtable",
"early_fixup_exception",
"do_machine_check",
"lbr_epilogue"
} ;
