#!/bin/sh
#
# link vmlinux
#
# vmlinux is linked from the objects selected by $(KBUILD_VMLINUX_INIT) and
# $(KBUILD_VMLINUX_MAIN). Most are built-in.o files from top-level directories
# in the kernel tree, others are specified in arch/$(ARCH)/Makefile.
# Ordering when linking is important, and $(KBUILD_VMLINUX_INIT) must be first.
#
# vmlinux
#   ^
#   |
#   +-< $(KBUILD_VMLINUX_INIT)
#   |   +--< init/version.o + more
#   |
#   +--< $(KBUILD_VMLINUX_MAIN)
#   |    +--< drivers/built-in.o mm/built-in.o + more
#   |
#   +-< ${kallsymso} (see description in KALLSYMS section)
#
# vmlinux version (uname -v) cannot be updated during normal
# descending-into-subdirs phase since we do not yet know if we need to
# update vmlinux.
# Therefore this step is delayed until just before final link of vmlinux.
#
# System.map is generated to document addresses of all kernel symbols

# Error out on error
set -e

#LTO added functionality

fix_kernel_builtin()
{
   ${LTO_DIS} kernel/built-in.bc
   head -4 kernel/built-in.ll > intermediary.ll
   cat .lto-fixup >> intermediary.ll
   awk '(!($1 ~ /^module$/)||!($2 ~ /^asm$/))&&(!($1 ~ /^;$/)||!($2 ~ /^ModuleID$/))&&(!($1 ~ /^source_filename$/)||!($2 ~ /^=$/))&&(!($1 ~ /^target$/)||!($2 ~ /^datalayout$/))&&(!($1 ~ /^target$/)||!($2 ~ /^triple$/)){print $0}' kernel/built-in.ll >> intermediary.ll
  
   ${LTO_AS} intermediary.ll -o kernel/built-in.bc

   echo 'true'
}

get_matching_suffix()
{
  declare -a element_list=("${!1}")
  local suffix=${2}
  local counter=0
  files_array=
  for i in "${element_list[@]}"
   do
        if [[ $i == *${suffix} ]]
        then
	    files_array[$counter]=$i
            let counter=counter+1
        fi
   done
   echo "${files_array[@]}"
}

replace_suffix()
{
     declare -a element_list=("${!1}")
     local from_prefix=${2}
     local to_prefix=${3}
     echo "${element_list[@]//$from_prefix/$to_prefix}"
}

get_existing_files()
{
   declare -a element_list=("${!1}")
   local counter=0
   files_array=
   for i in "${element_list[@]}"
   do
        if [ -e $i ]
        then
	    files_array[$counter]=$i
            let counter=counter+1
        fi
   done
   echo "${files_array[@]}"
}

get_nm_files()
{
   declare -a element_list=("${!1}")
   rm -f main_symbols
   touch main_symbols
   for i in "${element_list[@]}"
   do
       ${LLVM_NM} -g $i >> main_symbols
   done
   echo main_symbols
}

get_undefined_symbols()
{
   local m_symbols
   m_symbols=$(awk '!$3{printf "-u %s ",$2} $3{printf "-u %s ",$3}' main_symbols)
   echo ${m_symbols}
}

diff(){
  awk 'BEGIN{RS=ORS=" "}
       {NR==FNR?a[$0]++:a[$0]--}
       END{for(k in a)if(a[k])print k}' <(echo -n "${!1}") <(echo -n "${!2}")
}

get_intersection()
{
   declare -a element_list_1=("${!1}")
   declare -a element_list_2=("${!2}")
   Array3=($(diff element_list_1[@] element_list_2[@]))
   echo ${Array3[@]}
}

lto_modpost_link_ver1()
{
        local intermediate_array
        local LTO_KBUILD_VMLINUX_INIT
	local LTO_KBUILD_VMLINUX_MAIN
        local LTO_OUTPUT_FILE
        intermediate_array=(`echo ${KBUILD_VMLINUX_INIT}`);
        LTO_KBUILD_VMLINUX_INIT=`replace_suffix intermediate_array[@] .o .bc`
        intermediate_array=(`echo ${LTO_KBUILD_VMLINUX_INIT}`);
        LTO_KBUILD_VMLINUX_INIT=`get_existing_files intermediate_array[@]`

        intermediate_array=(`echo ${KBUILD_VMLINUX_MAIN}`);
        LTO_KBUILD_VMLINUX_MAIN=`replace_suffix intermediate_array[@] .o .bc`
        intermediate_array=(`echo ${LTO_KBUILD_VMLINUX_MAIN}`);
        LTO_KBUILD_VMLINUX_MAIN=`get_existing_files intermediate_array[@]`

        intermediate_array=(`echo ${1}`);
        LTO_OUTPUT_FILE=`replace_suffix intermediate_array[@] .o .bc`
	${LD} ${LDFLAGS} ${LTO_EXTRA_VMLINUX} -r -o ${LTO_OUTPUT_FILE} ${LTO_KBUILD_VMLINUX_INIT}                   \
		--start-group ${LTO_KBUILD_VMLINUX_MAIN} --end-group
}

lto_vmlinux_link_ver1()
{
        local intermediate_array
        local LTO_KBUILD_VMLINUX_INIT
	local LTO_KBUILD_VMLINUX_MAIN
        local LTO_OUTPUT_FILE
        local LTO_PARTIAL_VMLINUX_INIT
        local LTO_PARTIAL_VMLINUX_MAIN
        local LTO_PARTIAL_OUTPUT_FILE
        local LTO_LIBRARIES
        local KBUILD_VMLINUX_MAIN_AUX
        local LTO_ASSEMBLY_FILE
        local LTO_INTERMEDIATE_BC_TO_O
        local aux_array_1
        local aux_array_2
        local aux_string
        local symbols
        local vmlinux_init_array
        local vmlinux_main_array
        local counter=0

        KBUILD_VMLINUX_MAIN_AUX=${KBUILD_VMLINUX_MAIN}
        intermediate_array=(`echo ${KBUILD_VMLINUX_MAIN}`);
        LTO_LIBRARIES=`get_matching_suffix intermediate_array[@] .a`
        aux_array_1=(`echo ${LTO_LIBRARIES}`);
        KBUILD_VMLINUX_MAIN=`get_intersection intermediate_array[@] aux_array_1[@]`

        intermediate_array=(`echo ${KBUILD_VMLINUX_INIT}`);
        LTO_KBUILD_VMLINUX_INIT=`replace_suffix intermediate_array[@] .o .bc`
        intermediate_array=(`echo ${LTO_KBUILD_VMLINUX_INIT}`);
        LTO_KBUILD_VMLINUX_INIT=`get_existing_files intermediate_array[@]`

        aux_array_1=(`echo ${KBUILD_VMLINUX_INIT}`);
        aux_array_2=(`echo ${LTO_KBUILD_VMLINUX_INIT}`);
        aux_string=`replace_suffix aux_array_2[@] .bc .o`
        aux_array_2=(`echo ${aux_string}`);
        LTO_PARTIAL_VMLINUX_INIT=`get_intersection aux_array_1[@] aux_array_2[@]`

        intermediate_array=(`echo ${KBUILD_VMLINUX_MAIN}`);
        LTO_KBUILD_VMLINUX_MAIN=`replace_suffix intermediate_array[@] .o .bc`
        intermediate_array=(`echo ${LTO_KBUILD_VMLINUX_MAIN}`);
        LTO_KBUILD_VMLINUX_MAIN=`get_existing_files intermediate_array[@]`

        vmlinux_init_array=
        vmlinux_main_array=
        for i in ${LTO_KBUILD_VMLINUX_INIT}
        do
            ${LLC} -O2 -code-model=kernel -o ${i//.bc/.s_int} ${i}
            ${AS} --64 -o ${i//.bc/.o_int} ${i//.bc/.s_int}
	    vmlinux_init_array[$counter]=${i//.bc/.o_int}
            #rm -f ${i//.bc/.s_int}
            let counter=counter+1
        done
        LTO_KBUILD_VMLINUX_INIT=$(echo "${vmlinux_init_array[@]}")

        counter=0
        for i in ${LTO_KBUILD_VMLINUX_MAIN}
        do
            ${LLC} -O2 -code-model=kernel -o ${i//.bc/.s_int} ${i}
            ${AS} --64 -o ${i//.bc/.o_int} ${i//.bc/.s_int}
	    vmlinux_main_array[$counter]=${i//.bc/.o_int}
            #rm -f ${i//.bc/.s_int}
            let counter=counter+1
        done
        LTO_KBUILD_VMLINUX_MAIN=$(echo "${vmlinux_main_array[@]}")

        KBUILD_VMLINUX_MAIN=${KBUILD_VMLINUX_MAIN_AUX}

      	${LD} ${LDFLAGS}  ${LDFLAGS_vmlinux} -o ${1}                  \
			-T ${lds} ${LTO_PARTIAL_VMLINUX_INIT}  ${LTO_KBUILD_VMLINUX_INIT}                   \
			--start-group ${KBUILD_VMLINUX_MAIN} ${LTO_KBUILD_VMLINUX_MAIN} --end-group ${2}
        

        #KBUILD_VMLINUX_MAIN=${KBUILD_VMLINUX_MAIN_AUX}
}


lto_modpost_link()
{
        local intermediate_array
        local LTO_KBUILD_VMLINUX_INIT
	local LTO_KBUILD_VMLINUX_MAIN
        local LTO_OUTPUT_FILE
        local LTO_PARTIAL_VMLINUX_INIT
        local LTO_PARTIAL_VMLINUX_MAIN
        local LTO_PARTIAL_OUTPUT_FILE
        local LTO_LIBRARIES
        local KBUILD_VMLINUX_MAIN_AUX
        local LTO_ASSEMBLY_FILE
        local LTO_INTERMEDIATE_BC_TO_O
        local aux_array_1
        local aux_array_2
        local aux_string
        local symbols
        local vmlinux_init_array
        local counter=0
        local fix_kernel

        KBUILD_VMLINUX_MAIN_AUX=${KBUILD_VMLINUX_MAIN}
        intermediate_array=(`echo ${KBUILD_VMLINUX_MAIN}`);
        LTO_LIBRARIES=`get_matching_suffix intermediate_array[@] .a`
        aux_array_1=(`echo ${LTO_LIBRARIES}`);
        #KBUILD_VMLINUX_MAIN=`get_intersection intermediate_array[@] aux_array_1[@]`

        intermediate_array=(`echo ${KBUILD_VMLINUX_INIT}`);
        LTO_KBUILD_VMLINUX_INIT=`replace_suffix intermediate_array[@] .o .bc`
        intermediate_array=(`echo ${LTO_KBUILD_VMLINUX_INIT}`);
        LTO_KBUILD_VMLINUX_INIT=`get_existing_files intermediate_array[@]`

        aux_array_1=(`echo ${KBUILD_VMLINUX_INIT}`);
        aux_array_2=(`echo ${LTO_KBUILD_VMLINUX_INIT}`);
        aux_string=`replace_suffix aux_array_2[@] .bc .o`
        aux_array_2=(`echo ${aux_string}`);
        LTO_PARTIAL_VMLINUX_INIT=`get_intersection aux_array_1[@] aux_array_2[@]`

        intermediate_array=(`echo ${KBUILD_VMLINUX_MAIN}`);
        LTO_KBUILD_VMLINUX_MAIN=`replace_suffix intermediate_array[@] .o .bc`
        intermediate_array=(`echo ${LTO_KBUILD_VMLINUX_MAIN}`);
        LTO_KBUILD_VMLINUX_MAIN=`get_existing_files intermediate_array[@]`

        #aux_array_1=(`echo ${KBUILD_VMLINUX_MAIN}`);
        #aux_array_2=(`echo ${LTO_KBUILD_VMLINUX_MAIN}`);
        #aux_string=`replace_suffix aux_array_2[@] .bc .o`
        #aux_array_2=(`echo ${aux_string}`);
        #LTO_PARTIAL_VMLINUX_MAIN=`get_intersection aux_array_1[@] aux_array_2[@]`

        intermediate_array=(`echo ${1}`);
        LTO_OUTPUT_FILE=`replace_suffix intermediate_array[@] .o .int_bc`
        LTO_PARTIAL_OUTPUT_FILE=`replace_suffix intermediate_array[@] .o .int`
        LTO_ASSEMBLY_FILE=`replace_suffix intermediate_array[@] .o .int_s`
        LTO_INTERMEDIATE_BC_TO_O=`replace_suffix intermediate_array[@] .o .int_o`
        
        intermediate_array=(`echo ${KBUILD_VMLINUX_INIT}  ${LTO_KBUILD_VMLINUX_INIT}`);
	symbols=`get_nm_files intermediate_array[@]`
        symbols=`get_undefined_symbols`
        
        fix_kernel=`fix_kernel_builtin`

        rm -f empty.o
        rm -f begin.o
	${AR} rcsD empty.o
	${LD} -m elf_x86_64 ${symbols} -r -o begin.o empty.o
        
        for i in ${LTO_KBUILD_VMLINUX_INIT}
        do
            ${LLC} -O2 ${LTO_EXTRA_LLC_FLAGS} -o ${i//.bc/.s_int} ${i}
            ${AS} --64 -o ${i//.bc/.o_int} ${i//.bc/.s_int}
	    vmlinux_init_array[$counter]=${i//.bc/.o_int}
            #rm -f ${i//.bc/.s_int}
            let counter=counter+1
        done
        LTO_KBUILD_VMLINUX_INIT=$(echo "${vmlinux_init_array[@]}")

 
        ${LD} ${LDFLAGS} ${LTO_EXTRA_VMLINUX} -r -o ${LTO_OUTPUT_FILE} begin.o                   \
		 --start-group ${LTO_KBUILD_VMLINUX_MAIN} --end-group
        ${LLC} -O2 ${LTO_EXTRA_LLC_FLAGS} -o ${LTO_ASSEMBLY_FILE} ${LTO_OUTPUT_FILE}
	${AS} --64 -o ${LTO_INTERMEDIATE_BC_TO_O} ${LTO_ASSEMBLY_FILE}

        ${LTO_LD} ${LDFLAGS} -r -o ${1} ${LTO_PARTIAL_VMLINUX_INIT} ${LTO_KBUILD_VMLINUX_INIT}              \
		--start-group  ${KBUILD_VMLINUX_MAIN} ${LTO_INTERMEDIATE_BC_TO_O} --end-group

        #KBUILD_VMLINUX_MAIN=${KBUILD_VMLINUX_MAIN_AUX}
        
}

lto_vmlinux_link()
{
        local intermediate_array
        local LTO_KBUILD_VMLINUX_INIT
	local LTO_KBUILD_VMLINUX_MAIN
        local LTO_OUTPUT_FILE
        local LTO_PARTIAL_VMLINUX_INIT
        local LTO_PARTIAL_VMLINUX_MAIN
        local LTO_PARTIAL_OUTPUT_FILE
        local LTO_LIBRARIES
        local KBUILD_VMLINUX_MAIN_AUX
        local LTO_ASSEMBLY_FILE
        local LTO_INTERMEDIATE_BC_TO_O
        local aux_array_1
        local aux_array_2
        local aux_string
        local symbols
        local vmlinux_init_array
        local counter=0

        #KBUILD_VMLINUX_MAIN_AUX=${KBUILD_VMLINUX_MAIN}
        #intermediate_array=(`echo ${KBUILD_VMLINUX_MAIN}`);
        #LTO_LIBRARIES=`get_matching_suffix intermediate_array[@] .a`
        #aux_array_1=(`echo ${LTO_LIBRARIES}`);
        #KBUILD_VMLINUX_MAIN=`get_intersection intermediate_array[@] aux_array_1[@]`

        intermediate_array=(`echo ${KBUILD_VMLINUX_INIT}`);
        LTO_KBUILD_VMLINUX_INIT=`replace_suffix intermediate_array[@] .o .bc`
        intermediate_array=(`echo ${LTO_KBUILD_VMLINUX_INIT}`);
        LTO_KBUILD_VMLINUX_INIT=`get_existing_files intermediate_array[@]`

        aux_array_1=(`echo ${KBUILD_VMLINUX_INIT}`);
        aux_array_2=(`echo ${LTO_KBUILD_VMLINUX_INIT}`);
        aux_string=`replace_suffix aux_array_2[@] .bc .o`
        aux_array_2=(`echo ${aux_string}`);
        LTO_PARTIAL_VMLINUX_INIT=`get_intersection aux_array_1[@] aux_array_2[@]`

        intermediate_array=(`echo ${KBUILD_VMLINUX_MAIN}`);
        LTO_KBUILD_VMLINUX_MAIN=`replace_suffix intermediate_array[@] .o .bc`
        intermediate_array=(`echo ${LTO_KBUILD_VMLINUX_MAIN}`);
        LTO_KBUILD_VMLINUX_MAIN=`get_existing_files intermediate_array[@]`

        for i in ${LTO_KBUILD_VMLINUX_INIT}
        do
            ${LLC} -O2 ${LTO_EXTRA_LLC_FLAGS} -o ${i//.bc/.s_int} ${i}
            ${AS} --64 -o ${i//.bc/.o_int} ${i//.bc/.s_int}
	    vmlinux_init_array[$counter]=${i//.bc/.o_int}
            #rm -f ${i//.bc/.s_int}
            let counter=counter+1
        done
        LTO_KBUILD_VMLINUX_INIT=$(echo "${vmlinux_init_array[@]}")

        ${LD} ${LDFLAGS} ${LTO_EXTRA_VMLINUX} -r -o ${1}.int_bc begin.o                  \
		--start-group ${LTO_KBUILD_VMLINUX_MAIN} --end-group

        ${LLC} -O2 ${LTO_EXTRA_LLC_FLAGS} -o ${1}.int_s ${1}.int_bc
	${AS} --64 -o ${1}.int_o ${1}.int_s

      	${LTO_LD} ${LDFLAGS} ${LDFLAGS_vmlinux} -o ${1}                  \
			-T ${lds} ${LTO_PARTIAL_VMLINUX_INIT} ${LTO_KBUILD_VMLINUX_INIT}                   \
			--start-group ${KBUILD_VMLINUX_MAIN}  ${1}.int_o  --end-group ${2}

        #KBUILD_VMLINUX_MAIN=${KBUILD_VMLINUX_MAIN_AUX}
}


lto_modpost_link_ver2()
{
        local intermediate_array
        local LTO_KBUILD_VMLINUX_INIT
	local LTO_KBUILD_VMLINUX_MAIN
        local LTO_OUTPUT_FILE
        local LTO_PARTIAL_VMLINUX_INIT
        local LTO_PARTIAL_VMLINUX_MAIN
        local LTO_PARTIAL_OUTPUT_FILE
        local LTO_LIBRARIES
        local KBUILD_VMLINUX_MAIN_AUX
        local LTO_ASSEMBLY_FILE
        local LTO_INTERMEDIATE_BC_TO_O
        local aux_array_1
        local aux_array_2
        local aux_string
        local symbols

        KBUILD_VMLINUX_MAIN_AUX=${KBUILD_VMLINUX_MAIN}
        intermediate_array=(`echo ${KBUILD_VMLINUX_MAIN}`);
        LTO_LIBRARIES=`get_matching_suffix intermediate_array[@] .a`
        aux_array_1=(`echo ${LTO_LIBRARIES}`);
        #KBUILD_VMLINUX_MAIN=`get_intersection intermediate_array[@] aux_array_1[@]`

        intermediate_array=(`echo ${KBUILD_VMLINUX_INIT}`);
        LTO_KBUILD_VMLINUX_INIT=`replace_suffix intermediate_array[@] .o .bc`
        intermediate_array=(`echo ${LTO_KBUILD_VMLINUX_INIT}`);
        LTO_KBUILD_VMLINUX_INIT=`get_existing_files intermediate_array[@]`

        aux_array_1=(`echo ${KBUILD_VMLINUX_INIT}`);
        aux_array_2=(`echo ${LTO_KBUILD_VMLINUX_INIT}`);
        aux_string=`replace_suffix aux_array_2[@] .bc .o`
        aux_array_2=(`echo ${aux_string}`);
        LTO_PARTIAL_VMLINUX_INIT=`get_intersection aux_array_1[@] aux_array_2[@]`

        intermediate_array=(`echo ${KBUILD_VMLINUX_MAIN}`);
        LTO_KBUILD_VMLINUX_MAIN=`replace_suffix intermediate_array[@] .o .bc`
        intermediate_array=(`echo ${LTO_KBUILD_VMLINUX_MAIN}`);
        LTO_KBUILD_VMLINUX_MAIN=`get_existing_files intermediate_array[@]`

        #aux_array_1=(`echo ${KBUILD_VMLINUX_MAIN}`);
        #aux_array_2=(`echo ${LTO_KBUILD_VMLINUX_MAIN}`);
        #aux_string=`replace_suffix aux_array_2[@] .bc .o`
        #aux_array_2=(`echo ${aux_string}`);
        #LTO_PARTIAL_VMLINUX_MAIN=`get_intersection aux_array_1[@] aux_array_2[@]`

        intermediate_array=(`echo ${1}`);
        LTO_OUTPUT_FILE=`replace_suffix intermediate_array[@] .o .int_bc`
        LTO_PARTIAL_OUTPUT_FILE=`replace_suffix intermediate_array[@] .o .int`
        LTO_ASSEMBLY_FILE=`replace_suffix intermediate_array[@] .o .int_s`
        LTO_INTERMEDIATE_BC_TO_O=`replace_suffix intermediate_array[@] .o .int_o`
        
        intermediate_array=(`echo ${KBUILD_VMLINUX_INIT}  ${LTO_KBUILD_VMLINUX_INIT}`);
	symbols=`get_nm_files intermediate_array[@]`
        symbols=`get_undefined_symbols`
        
        rm -f empty.o
        rm -f begin.o
	${AR} rcsD empty.o
	${LD} -m elf_x86_64 ${symbols} -r -o begin.o empty.o

 
        ${LD} ${LDFLAGS} ${LTO_EXTRA_VMLINUX} -r -o ${LTO_OUTPUT_FILE} begin.o ${LTO_KBUILD_VMLINUX_INIT}                   \
		${LTO_KBUILD_VMLINUX_MAIN}
        ${LLC} -O2 ${LTO_EXTRA_LLC_FLAGS} -o ${LTO_ASSEMBLY_FILE} ${LTO_OUTPUT_FILE}
	${AS} --64 -o ${LTO_INTERMEDIATE_BC_TO_O} ${LTO_ASSEMBLY_FILE}

        ${LD} ${LDFLAGS}  -r -o ${1} ${LTO_PARTIAL_VMLINUX_INIT}  ${LTO_INTERMEDIATE_BC_TO_O}  begin.o                  \
		--start-group ${KBUILD_VMLINUX_MAIN} --end-group

        #KBUILD_VMLINUX_MAIN=${KBUILD_VMLINUX_MAIN_AUX}
        
}

lto_vmlinux_link_ver2()
{
        local intermediate_array
        local LTO_KBUILD_VMLINUX_INIT
	local LTO_KBUILD_VMLINUX_MAIN
        local LTO_OUTPUT_FILE
        local LTO_PARTIAL_VMLINUX_INIT
        local LTO_PARTIAL_VMLINUX_MAIN
        local LTO_PARTIAL_OUTPUT_FILE
        local LTO_LIBRARIES
        local KBUILD_VMLINUX_MAIN_AUX
        local LTO_ASSEMBLY_FILE
        local LTO_INTERMEDIATE_BC_TO_O
        local aux_array_1
        local aux_array_2
        local aux_string
        local symbols

        #KBUILD_VMLINUX_MAIN_AUX=${KBUILD_VMLINUX_MAIN}
        #intermediate_array=(`echo ${KBUILD_VMLINUX_MAIN}`);
        #LTO_LIBRARIES=`get_matching_suffix intermediate_array[@] .a`
        #aux_array_1=(`echo ${LTO_LIBRARIES}`);
        #KBUILD_VMLINUX_MAIN=`get_intersection intermediate_array[@] aux_array_1[@]`

        intermediate_array=(`echo ${KBUILD_VMLINUX_INIT}`);
        LTO_KBUILD_VMLINUX_INIT=`replace_suffix intermediate_array[@] .o .bc`
        intermediate_array=(`echo ${LTO_KBUILD_VMLINUX_INIT}`);
        LTO_KBUILD_VMLINUX_INIT=`get_existing_files intermediate_array[@]`

        aux_array_1=(`echo ${KBUILD_VMLINUX_INIT}`);
        aux_array_2=(`echo ${LTO_KBUILD_VMLINUX_INIT}`);
        aux_string=`replace_suffix aux_array_2[@] .bc .o`
        aux_array_2=(`echo ${aux_string}`);
        LTO_PARTIAL_VMLINUX_INIT=`get_intersection aux_array_1[@] aux_array_2[@]`

        intermediate_array=(`echo ${KBUILD_VMLINUX_MAIN}`);
        LTO_KBUILD_VMLINUX_MAIN=`replace_suffix intermediate_array[@] .o .bc`
        intermediate_array=(`echo ${LTO_KBUILD_VMLINUX_MAIN}`);
        LTO_KBUILD_VMLINUX_MAIN=`get_existing_files intermediate_array[@]`

        ${LD} ${LDFLAGS} ${LTO_EXTRA_VMLINUX} -r -o ${1}.int_bc begin.o ${LTO_KBUILD_VMLINUX_INIT}                   \
		--start-group ${LTO_KBUILD_VMLINUX_MAIN} --end-group

        ${LLC} -O2 ${LTO_EXTRA_LLC_FLAGS} -o ${1}.int_s ${1}.int_bc
	${AS} --64 -o ${1}.int_o ${1}.int_s

      	${LD} ${LDFLAGS}  ${LDFLAGS_vmlinux} -o ${1}                  \
			-T ${lds} ${LTO_PARTIAL_VMLINUX_INIT}  ${1}.int_o                   \
			--start-group ${KBUILD_VMLINUX_MAIN} --end-group ${2}

        #KBUILD_VMLINUX_MAIN=${KBUILD_VMLINUX_MAIN_AUX}
}


# Nice output in kbuild format
# Will be supressed by "make -s"
info()
{
	if [ "${quiet}" != "silent_" ]; then
		printf "  %-7s %s\n" ${1} ${2}
	fi
}

# Link of vmlinux.o used for section mismatch analysis
# ${1} output file
modpost_link()
{
	if [ "${HAVE_LTO}" == "True" ]; then
                        lto_modpost_link ${1}
        else
		 ${LD} ${LDFLAGS}  -r -o ${1} ${KBUILD_VMLINUX_INIT}                   \
		--start-group ${KBUILD_VMLINUX_MAIN} --end-group
        fi

#        if [ "${HAVE_LTO}" == "True" ]; then
#                        lto_modpost_link ${1}
#                     fi
}

# Link of vmlinux
# ${1} - optional extra .o files
# ${2} - output file
vmlinux_link()
{
	local lds="${objtree}/${KBUILD_LDS}"

	if [ "${SRCARCH}" != "um" ]; then
                if [ "${HAVE_LTO}" == "True" ]; then
                     lto_vmlinux_link ${2} ${1}
                else
		${LD} ${LDFLAGS} ${LTO_EXTRA_VMLINUX} ${LDFLAGS_vmlinux} -o ${2}                  \
			-T ${lds} ${KBUILD_VMLINUX_INIT}                     \
			--start-group ${KBUILD_VMLINUX_MAIN} --end-group ${1}
                fi
	else
		${CC} ${CFLAGS_vmlinux} -o ${2}                              \
			-Wl,-T,${lds} ${KBUILD_VMLINUX_INIT}                 \
			-Wl,--start-group                                    \
				 ${KBUILD_VMLINUX_MAIN}                      \
			-Wl,--end-group                                      \
			-lutil -lrt -lpthread ${1}
		rm -f linux
	fi
}


# Create ${2} .o file with all symbols from the ${1} object file
kallsyms()
{
	info KSYM ${2}
	local kallsymopt;

	if [ -n "${CONFIG_HAVE_UNDERSCORE_SYMBOL_PREFIX}" ]; then
		kallsymopt="${kallsymopt} --symbol-prefix=_"
	fi

	if [ -n "${CONFIG_KALLSYMS_ALL}" ]; then
		kallsymopt="${kallsymopt} --all-symbols"
	fi

	if [ -n "${CONFIG_ARM}" ] && [ -z "${CONFIG_XIP_KERNEL}" ] && [ -n "${CONFIG_PAGE_OFFSET}" ]; then
		kallsymopt="${kallsymopt} --page-offset=$CONFIG_PAGE_OFFSET"
	fi

	if [ -n "${CONFIG_X86_64}" ]; then
		kallsymopt="${kallsymopt} --absolute-percpu"
	fi

	local aflags="${KBUILD_AFLAGS} ${KBUILD_AFLAGS_KERNEL}               \
		      ${NOSTDINC_FLAGS} ${LINUXINCLUDE} ${KBUILD_CPPFLAGS}"

	${NM} -n ${1} | \
		scripts/kallsyms ${kallsymopt} | \
		${CC} ${aflags} -c -o ${2} -x assembler-with-cpp -
}

# Create map file with all symbols from ${1}
# See mksymap for additional details
mksysmap()
{
	${CONFIG_SHELL} "${srctree}/scripts/mksysmap" ${1} ${2}
}

sortextable()
{
	${objtree}/scripts/sortextable ${1}
}

# Delete output files in case of error
cleanup()
{
	rm -f .old_version
	rm -f .tmp_System.map
	rm -f .tmp_kallsyms*
	rm -f .tmp_version
	#rm -f .tmp_vmlinux*
	rm -f System.map
	rm -f vmlinux
	rm -f vmlinux.o
}

on_exit()
{
	if [ $? -ne 0 ]; then
		cleanup
	fi
}
trap on_exit EXIT

on_signals()
{
	exit 1
}
trap on_signals HUP INT QUIT TERM

#
#
# Use "make V=1" to debug this script
case "${KBUILD_VERBOSE}" in
*1*)
	set -x
	;;
esac

if [ "$1" = "clean" ]; then
	cleanup
	exit 0
fi

# We need access to CONFIG_ symbols
case "${KCONFIG_CONFIG}" in
*/*)
	. "${KCONFIG_CONFIG}"
	;;
*)
	# Force using a file from the current directory
	. "./${KCONFIG_CONFIG}"
esac

#link vmlinux.o
info LD vmlinux.o
modpost_link vmlinux.o

# modpost vmlinux.o to check for section mismatches
${MAKE} -f "${srctree}/scripts/Makefile.modpost" vmlinux.o

# Update version
info GEN .version
if [ ! -r .version ]; then
	rm -f .version;
	echo 1 >.version;
else
	mv .version .old_version;
	expr 0$(cat .old_version) + 1 >.version;
fi;

# final build of init/
${MAKE} -f "${srctree}/scripts/Makefile.build" obj=init

kallsymso=""
kallsyms_vmlinux=""
if [ -n "${CONFIG_KALLSYMS}" ]; then

	# kallsyms support
	# Generate section listing all symbols and add it into vmlinux
	# It's a three step process:
	# 1)  Link .tmp_vmlinux1 so it has all symbols and sections,
	#     but __kallsyms is empty.
	#     Running kallsyms on that gives us .tmp_kallsyms1.o with
	#     the right size
	# 2)  Link .tmp_vmlinux2 so it now has a __kallsyms section of
	#     the right size, but due to the added section, some
	#     addresses have shifted.
	#     From here, we generate a correct .tmp_kallsyms2.o
	# 2a) We may use an extra pass as this has been necessary to
	#     woraround some alignment related bugs.
	#     KALLSYMS_EXTRA_PASS=1 is used to trigger this.
	# 3)  The correct ${kallsymso} is linked into the final vmlinux.
	#
	# a)  Verify that the System.map from vmlinux matches the map from
	#     ${kallsymso}.

	kallsymso=.tmp_kallsyms2.o
	kallsyms_vmlinux=.tmp_vmlinux2

	# step 1
	vmlinux_link "" .tmp_vmlinux1
	kallsyms .tmp_vmlinux1 .tmp_kallsyms1.o

	# step 2
	vmlinux_link .tmp_kallsyms1.o .tmp_vmlinux2
	kallsyms .tmp_vmlinux2 .tmp_kallsyms2.o

	# step 2a
	if [ -n "${KALLSYMS_EXTRA_PASS}" ]; then
		kallsymso=.tmp_kallsyms3.o
		kallsyms_vmlinux=.tmp_vmlinux3

		vmlinux_link .tmp_kallsyms2.o .tmp_vmlinux3

		kallsyms .tmp_vmlinux3 .tmp_kallsyms3.o
	fi
fi

info LD vmlinux
vmlinux_link "${kallsymso}" vmlinux

if [ -n "${CONFIG_BUILDTIME_EXTABLE_SORT}" ]; then
	info SORTEX vmlinux
	sortextable vmlinux
fi

info SYSMAP System.map
mksysmap vmlinux System.map

# step a (see comment above)
if [ -n "${CONFIG_KALLSYMS}" ]; then
	mksysmap ${kallsyms_vmlinux} .tmp_System.map

	if ! cmp -s System.map .tmp_System.map; then
		echo >&2 Inconsistent kallsyms data
		echo >&2 Try "make KALLSYMS_EXTRA_PASS=1" as a workaround
		exit 1
	fi
fi

# We made a new kernel - delete old version file
rm -f .old_version
