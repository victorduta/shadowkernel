#!/bin/bash

. global_exports.sh


num_nop=0
function get_num_nops {
     local num=0
     NOPS=$(awk '/'"$1"'/{flag=1; nr=0; getline;} flag&&nr<10{print $3;nr++} nr>10{exit}'  $QEMU_DIR/$KERNEL_VMLINUX_DUMP)
     for NOP in $NOPS; do
         if [ $NOP == 'nop' ]; then
             let num=num+1
         else
             break
         fi
     done
     let num_nop=num
}

#CALLS=$(cat ${QEMU_DIR}/${KERNEL_VMLINUX_DUMP}| grep call)
CALLS=$(cat ${QEMU_DIR}/call_stats.dump)
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

echo "Functions and their call size" > call.table
for(( c=0; c<$i ; c++ ))
do
   num_nop=0
   get_num_nops ${CALL_MATCH_TOKEN[$c]}

#   if (( 8 == $num_nop + ${CALL_SIZE[$c]} )); then
#       if [ $1 == 'print_all' ]; then
#       echo -n ${CALL_MATCH_TOKEN[$c]} >> call.table 
#       echo -n "     "  >> call.table
#       echo -n ${CALL_FORMAT[$c]} >> call.table
#       echo -n "     " >> call.table 
#       echo -n ${CALL_SIZE[$c]}  >> call.table
#       echo -n "     " >> call.table
#       echo -n ${num_nop} >> call.table
#       echo -n "     " >> call.table 
#       echo "SUCCESS" >> call.table
#       fi
#   else
 if (( 8 != $num_nop + ${CALL_SIZE[$c]} )); then
       echo -n ${CALL_MATCH_TOKEN[$c]} >> call.table 
       echo -n "     "  >> call.table
       echo -n ${CALL_FORMAT[$c]} >> call.table
       echo -n "     " >> call.table 
       echo -n ${CALL_SIZE[$c]}  >> call.table
       echo -n "     " >> call.table
       echo -n ${num_nop} >> call.table
       echo -n "     " >> call.table 
       echo "FAILED" >> call.table
   fi
done
