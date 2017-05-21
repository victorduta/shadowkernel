#!/bin/bash
. global_exports.sh

RETURN_TO=$PWD #return back to where the script gets executed

cp ${MODULE_PATH}/${MODULE_NAME} ${ROOTFS_DIR}/home/
cp ${MODULE_PATH}/${MKNOD_SCRIPT} ${ROOTFS_DIR}/home/
cp ${MODULE_PATH}/${IOCTL_SCRIPT} ${ROOTFS_DIR}/home/

cd ${ROOTFS_DIR} 

rm -f ../${ROOTFS_IMG}


find . | cpio -o --format=newc > ../${ROOTFS_IMG}


cd $RETURN_TO




