#!/bin/bash
MAJOR=10
MINOR=$1
DEVNAME=shadow_lbr

#sudo mknod /dev/$DEVNAME c $MAJOR $MINOR
sudo chmod a+r+w /dev/$DEVNAME 


#mknod /dev/$DEVNAME c $MAJOR $MINOR
#chmod a+r+w /dev/$DEVNAME
