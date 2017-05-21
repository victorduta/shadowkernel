. global_exports.sh
i=0
num=0;
for((num=1; num <= $#; num++))
do
    CALL_GRAPH[$i]=${!num}
    let i=i+1
done
function add_element {
                local c=0
                for(( c=0; c<$i ; c++ ))
                do
                     if [ $1 == ${CALL_GRAPH[$c]} ]; then
                        echo "${1} already exists in cg"
                        return
                     fi 
                done
                CALL_GRAPH[$i]=$1
                let i=i+1
                echo "adding ${1} to cg"
           }
function get_calls {
               FUNCTION_CALLS=$(./strip_function.sh $1 | awk '{print $NF}')
               for FUNCTION in $FUNCTION_CALLS; do
                    add_element $FUNCTION
               done 
           }  
function print_cg {
                for(( c=0; c<$i ; c++ ))
                do
                    echo ${CALL_GRAPH[$c]}
                done
         }
function generate_exclude_header {
             local c=0
             rm -f ${PASS_INCLUDE_DIR}/${PASS_EXCLUDE_HEADER}
             touch ${PASS_INCLUDE_DIR}/${PASS_EXCLUDE_HEADER}
             echo "vector<string> jump_functions = {" >> ${PASS_INCLUDE_DIR}/${PASS_EXCLUDE_HEADER}
             for(( c=0; c<$i ; c++ ))
             do
                if (( $c == $i-1 )); then
                   echo "\""${CALL_GRAPH[$c]}"\"}" >>  ${PASS_INCLUDE_DIR}/${PASS_EXCLUDE_HEADER}
                else
                   echo "\""${CALL_GRAPH[$c]}"\"," >>  ${PASS_INCLUDE_DIR}/${PASS_EXCLUDE_HEADER}
                fi
             done
 
         }


#if false;then
index=0
while true; do
    get_calls ${CALL_GRAPH[$index]}
    if (($index == $i)); then
       break
    fi
    let index=index+1
done
#fi

generate_exclude_header

