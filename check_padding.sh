#!/bin/bash

. global_exports.sh



NOPS=$(awk 'flag&&$3=="nop"{nr++} (flag==1)&&($3!="nop"){flag=0; print nr} /call/{flag=1; nr=0}'  ${QEMU_DIR}/${KERNEL_VMLINUX_DUMP}) 
CALLS=$(cat ${QEMU_DIR}/${KERNEL_VMLINUX_DUMP}| grep call)
i=0
after_call=2
for CALL in $CALLS; do
     case $CALL in
       *: ) 
       CALL_MATCH_TOKEN[$i]=$CALL
       CALL_SIZE[$i]=0
       after_call=0
       ;;
       *call* )
       
       after_call=1
       ;;
       * )
       if (( $after_call == 0 )); then
           let CALL_SIZE[$i]=CALL_SIZE[$i]+1
       elif(( $after_call == 1 )); then
           CALL_FORMAT[$i]=$CALL
           let i=i+1
           after_call=2
       fi 
       ;;
     esac
done
rm -f result.table 
touch result.table
j=0
let NOP_VEC[$j]=0
for NOP in $NOPS; do
   let NOP_VEC[$j]=$NOP
   let j=j+1
done


for(( c=0; c<$i ; c++ ))
do
  if (( 8 != ${NOP_VEC[$c]} + ${CALL_SIZE[$c]} )); then
       echo -n ${CALL_MATCH_TOKEN[$c]} >> result.table 
       echo -n "     "  >> result.table
       echo -n ${CALL_FORMAT[$c]} >> result.table
       echo -n "     " >> result.table 
       echo -n ${CALL_SIZE[$c]}  >> result.table
       echo -n "     " >> result.table 
       echo -n ${NOP_VEC[$c]}  >> result.table
       echo -n "     " >> result.table 
       echo "FAILED" >> result.table
  fi
done

