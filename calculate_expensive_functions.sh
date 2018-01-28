#!/bin/bash

. global_exports.sh

PARSE=$(awk '($2 >= 400000){print $1}' $1) 

for TOKEN in $PARSE; do
  TOKEN_STRING=$(awk '$1 ~ /^'"$TOKEN"'$/{print $3}' $QEMU_DIR/$KERNEL_MAP)
  for INDEPENDENT_TOKEN in ${TOKEN_STRING}; do
      echo "\""${INDEPENDENT_TOKEN}"\","
  done
done
