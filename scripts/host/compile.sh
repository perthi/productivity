#!/bin/bash

if [ $# -ne 1 ];then
    echo "Usage:" $0 "[target] wher target is either x86, arm or clean"
    exit -1
fi

image_name=cpp-cross-compiler

if [ $1 = "x86" ] || [ $1 = "arm" ] || [ $1 = "clean" ]; then
    docker run --rm -v ${PWD}:/home/root/ --workdir /home/root/ $image_name "./scripts/docker/build.sh" $1
    exit $?;    
else
    echo "Unknown target " $1  "! Aborting build.."
    exit 3
fi    

exit 0
