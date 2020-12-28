#!/bin/bash

image_name=cpp-cross-compiler

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu/

if [ $1 = "x86" ] || [ $1 = "arm" ] || [ $1 = "clean" ]; then
    if [ $1 = "x86"  ]; then
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu/ 
    fi
    if [ $1 = "arm"  ]; then
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/arm-linux-gnueabi/libhf/ 
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/arm-linux-gnueabihf/lib/
    fi
    
    docker run --user root --rm -v ${PWD}:/home/root/ --workdir /home/root/ $image_name "./scripts/docker/run-unittests.sh" $1 $2 $3 $4 

    exit $?;    
else
    echo "Unknown target " $1  "! Aborting build.."
    exit 3
fi    

exit 0
