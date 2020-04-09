#!/bin/bash
git pull
./touch-all.sh
make distclean
make x86 -j

