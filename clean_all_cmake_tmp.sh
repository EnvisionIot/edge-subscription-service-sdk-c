#!/bin/bash

rm_by_name()
{
    if [ $# -ge 1 ]
    then
        find_result=`find ./ -name "$1"`
#        echo "$find_result"
        for element in ${find_result}
        do
            echo "rm -rf ${element}"
            rm -rf "${element}"
        done
    fi
}

rm_by_name "CMakeFiles"
rm_by_name "cmake_install.cmake"
rm_by_name "CMakeCache.txt"
rm_by_name "Makefile"