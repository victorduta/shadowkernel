
echo $PWD
. global_exports.sh
#qemu-system-x86_64  -gdb tcp::1234 -kernel ${QEMU_DIR}/${KERNEL_VMLINUX} -initrd  ${QEMU_DIR}/${KERNEL_INITRD} -append "loglevel=1" -m 512M

. make_rootfs.sh

#sudo qemu-system-x86_64  -gdb tcp::1234 -kernel ${QEMU_DIR}/${KERNEL_VMLINUX} -initrd  ${QEMU_DIR}/${ROOTFS_IMG} -append "loglevel=1 root=/dev/ram rdinit=/sbin/init" -m 2G  # uses our busy-box rotfs


sudo qemu-system-x86_64 -cpu Nehalem -gdb tcp::1234 -kernel ${QEMU_DIR}/${KERNEL_VMLINUX} -initrd  ${QEMU_DIR}/${ROOTFS_IMG} -append "loglevel=1 root=/dev/ram rdinit=/sbin/init" -m 2G  # uses our busy-box rotfs
