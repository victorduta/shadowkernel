
. global_exports.sh

if [ $# -eq 1 ];then
   K_MAP=$QEMU_DIR/$KERNEL_MAP
   K_DUMP=$QEMU_DIR/$KERNEL_VMLINUX_DUMP
else
   echo "We supply map and dump"
   K_MAP=${2}
   K_DUMP=${3}
fi

#echo "Begining/End of function ${1}"
FIRST_LINE=$(awk '$3 ~ /^'"${1}"'$/{print $1}' $K_MAP)
SECOND_LINE=$(awk '$3 ~ /^'"${1}"'$/{getline; print $1}' $K_MAP)

echo $FIRST_LINE
echo $SECOND_LINE

awk '/'"$FIRST_LINE"':/{flag=1}/'"$SECOND_LINE"':/{flag=0}flag {print $0}'  $K_DUMP > dumper.code


#echo "Entire function is"
#cat dumper.code

cat dumper.code
rm dumper.code
