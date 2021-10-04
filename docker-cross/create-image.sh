#!/bin/bash

docker build --network=host --tag cpp-cross-compiler .

ret=$?

if [ ! $ret -eq 0 ]
then
    echo "Error creating image !!"    >&2  
    exit $ret
else
    echo "Sucessfully created image"  >&2
    exit 0
fi  


exit 0

