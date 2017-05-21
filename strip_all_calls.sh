
. global_exports.sh

#echo "Begining/End of function ${1}"
FIRST_LINE=$(awk '$3 ~ /^'"${1}"'$/{print $1}' $QEMU_DIR/$KERNEL_MAP)
SECOND_LINE=$(awk '$3 ~ /^'"${1}"'$/{getline; print $1}' $QEMU_DIR/$KERNEL_MAP)

#echo $FIRST_LINE
#echo $SECOND_LINE

awk '/'"$FIRST_LINE"':/{flag=1}/'"$SECOND_LINE"':/{flag=0}flag {print $0}'  $QEMU_DIR/$KERNEL_VMLINUX_DUMP > dumper.code

cat dumper.code | grep callq > dumper.calls

#echo "Entire function is"
#cat dumper.code

cat dumper.calls
rm dumper.code
rm dumper.calls

