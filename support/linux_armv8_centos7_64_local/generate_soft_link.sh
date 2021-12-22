#!/bin/bash
cd lib
files=$(ls)
for filename in $files
do
    OLD_IFS="$IFS"
    IFS="."
    arr=($filename)
    IFS="$OLD_IFS"
    arr_len=${#arr[@]}
    if [ $arr_len -ge 4 ]
    then
        if [[ ${arr[0]} == liblog4cplus-* ]]
        then
            new_name=${arr[0]}"."${arr[1]}"."${arr[2]}"."${arr[3]}
            ln -s "${filename}" "${new_name}" >> /dev/null 2>&1
            new_name=${arr[0]}"."${arr[1]}"."${arr[2]}
            ln -s "${filename}" "${new_name}" >> /dev/null 2>&1
            new_name="liblog4cplus.so"
            ln -s "${filename}" "${new_name}" >> /dev/null 2>&1
        else
            new_name=${arr[0]}"."${arr[1]}"."${arr[2]}
            ln -s "${filename}" "${new_name}" >> /dev/null 2>&1
            new_name=${arr[0]}"."${arr[1]}
            ln -s "${filename}" "${new_name}" >> /dev/null 2>&1
        fi
    elif [ $arr_len -ge 3 ]
    then
        if [[ ${arr[0]} == liblog4cplus-* ]]
        then
            new_name="liblog4cplus.so"
            ln -s "${filename}" "${new_name}" >> /dev/null 2>&1
        else
            new_name=${arr[0]}"."${arr[1]}
            ln -s "${filename}" "${new_name}" >> /dev/null 2>&1
        fi
    fi
done
exit 0
