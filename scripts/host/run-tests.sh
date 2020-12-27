#!/bin/bash

image_name=cpp-cross-compiler

if [ $1 = "x86" ] || [ $1 = "arm" ] || [ $1 = "clean" ]; then
    docker run --user root --rm -v ${PWD}:/home/root/ --workdir /home/root/ $image_name "./scripts/docker/run-unittests.sh" $1 $2 $3 $4 
    exit $?;    
else
    echo "Unknown target " $1  "! Aborting build.."
    exit 3
fi    

exit 0
