
echo $PWD
. global_exports.sh
qemu-system-x86_64  -kernel ${QEMU_DIR}/${KERNEL_VMLINUX} -initrd  ${QEMU_DIR}/${KERNEL_INITRD} -append "loglevel=7 console=tty0 console=ttyS0 rw" -serial stdio -m 2G

#qemu-system-x86_64  -kernel ${PWD}/test/kernel_compare/no_lto_kernel/vmlinuz-4.4.17 -initrd  ${PWD}/test/kernel_compare/no_lto_kernel/initrd.img-4.4.17 -append "loglevel=7 console=tty0 console=ttyS0 rw" -serial stdio -m 2G

#. make_rootfs.sh

#sudo qemu-system-x86_64  -gdb tcp::1234 -kernel ${PWD}/test/kernel_compare/no_lto_kernel/vmlinuz-4.4.17 -initrd  ${QEMU_DIR}/${ROOTFS_IMG} -append "loglevel=1 root=/dev/ram rdinit=/sbin/init" -m 2G  # uses our busy-box rotfs


#sudo qemu-system-x86_64 -cpu Nehalem -gdb tcp::1234 -kernel ${QEMU_DIR}/${KERNEL_VMLINUX} -initrd  ${QEMU_DIR}/${ROOTFS_IMG} -append "loglevel=1 root=/dev/ram rdinit=/sbin/init" -m 2G  # uses our busy-box rotfs

#sudo qemu-system-x86_64 -cpu Nehalem -gdb tcp::1234 -kernel ${QEMU_DIR}/${KERNEL_VMLINUX} -initrd  ${QEMU_DIR}/${ROOTFS_IMG} -append "loglevel=1 root=/dev/ram rdinit=/sbin/init loglevel=7 console=tty0 console=ttyS0" -serial stdio -m 2G  # uses our busy-box rotfs
