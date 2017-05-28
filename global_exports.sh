#!/bin/bash
export KERNEL_VERSION=4.4.17
export BASE_EXTRA_VERSION=1
export HEADERS_VERSION=4.4.0-71
export KERNEL_DIR=/boot
export KERNEL_VMLINUX=vmlinuz-$KERNEL_VERSION
export KERNEL_CONFIG=config-$KERNEL_VERSION
export KERNEL_INITRD=initrd.img-$KERNEL_VERSION
export KERNEL_MAP=System.map-$KERNEL_VERSION
export KERNEL_VMLINUX_OBJECT=vmlinux
export KERNEL_VMLINUX_DUMP=vmlinux_dump
export QEMU_DIR=$PWD/llvm-kernel
export PASS_INCLUDE_DIR=$PWD/llvm-includes
export PASS_EXCLUDE_HEADER=exclude_functions.h
export LIB_DIR=/lib/modules/${KERNEL_VERSION}
export EXTRACT_VMLINUX=/usr/src/linux-headers-$HEADERS_VERSION/scripts/extract-vmlinux



# The following environment variables are used by make_rootfs.sh to make a rootfs for qemu execution
export ROOTFS_DIR=${QEMU_DIR}/rootfs
export ROOTFS_IMG=rootfs.img
export MODULE_NAME=shadow-module.ko
export MODULE_PATH=$PWD/lbr_module
export MKNOD_SCRIPT=mknod.sh
export IOCTL_SCRIPT=try_ioctl
