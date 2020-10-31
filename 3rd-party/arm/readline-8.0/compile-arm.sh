#!/bin/bash
make clean
export CC=arm-linux-gnueabihf-gcc
./configure --host=arm-linux --target=arm-linux
make -j16
mv *.a lib/
rm *.o
