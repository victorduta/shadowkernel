#!/bin/bash
. global_exports.sh

# Patch in case we clean base kernel build
if [ $1 == baseKernel ]; then
  LIB_DIR=$LIB_DIR.1
  KERNEL_VMLINUX=${KERNEL_VMLINUX}.${BASE_EXTRA_VERSION}
  KERNEL_CONFIG=${KERNEL_CONFIG}.${BASE_EXTRA_VERSION}
  KERNEL_INITRD=${KERNEL_INITRD}.${BASE_EXTRA_VERSION}
  KERNEL_MAP=${KERNEL_MAP}.${BASE_EXTRA_VERSION}
fi



sudo rm -r -f $LIB_DIR
sudo rm -f ${KERNEL_DIR}/${KERNEL_VMLINUX}
sudo rm -f ${KERNEL_DIR}/${KERNEL_CONFIG}
sudo rm -f ${KERNEL_DIR}/${KERNEL_INITRD}
sudo rm -f ${KERNEL_DIR}/${KERNEL_MAP}
sudo update-grub2
