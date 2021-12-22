#!/usr/bin/bash
SOURCE="$0"
while [ -h "$SOURCE"  ]; do # resolve $SOURCE until the file is no longer a symlink
    DIR="$( cd -P "$( dirname "$SOURCE"  )" && pwd  )"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /*  ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE"  )" && pwd  )"

if [ $LD_LIBRARY_PATH ]
then
    export LD_LIBRARY_PATH=$DIR/dst/lib/3rd:$DIR/dst/lib:$DIR/dst/lib/support:$LD_LIBRARY_PATH
else
    export LD_LIBRARY_PATH=$DIR/dst/lib/3rd:$DIR/dst/lib:$DIR/dst/lib/support
fi

cd $DIR/dst/test

./edge_dataservice_benchmark_test $@
