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
  LAST_TOKEN=""
  for INDEPENDENT_TOKEN in ${TOKEN_STRING}; do
      LAST_TOKEN=${INDEPENDENT_TOKEN}
  done
  CALL_WRAPPER=$(./strip_entire_function.sh ${LAST_TOKEN}| grep "add    %gs:0xa108,%rax" ) #this might change on compilation | grep "add    %gs:0xa108,%rax"
  for INDEPENDENT_TOKEN in ${TOKEN_STRING}; do
      [[ !  -z  $CALL_WRAPPER  ]] &&  echo "\""${INDEPENDENT_TOKEN}"\","
  done
done
