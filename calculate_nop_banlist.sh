#!/bin/bash

. global_exports.sh

awk '/0     FAILED/{print $2}' result.table | sort | uniq -c > result.uniq
awk '{print $2}' result.uniq > result.find
rm result.uniq
#cat result.find
PARSE=$(cat result.find| grep 0xff|  sed 's/0x//g') # grep first, we cannot find those based on registers
rm result.find
for TOKEN in $PARSE; do
  TOKEN_STRING=$(awk '$1 ~ /^'"$TOKEN"'$/{print $3}' $QEMU_DIR/$KERNEL_MAP)
  CALL_WRAPPER=$(./strip_all_calls.sh ${TOKEN_STRING} | grep 0xffffffff810002c0) #this might change on compilation
  [[ !  -z  $CALL_WRAPPER  ]] && echo "\""${TOKEN_STRING}"\","
done
