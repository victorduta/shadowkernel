

function check_from_branches {
     local num=0
     RESULTS=$(awk ' flag{nr++} $1 ~ /^TO$/{printf("%s %d\n",address,nr);flag=1; nr=0; address=$2; getline;} ' $1)
     echo ${RESULTS} > ujkl
}

check_from_branches $1
