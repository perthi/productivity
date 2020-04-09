#!/bin/bash
find  *  -exec touch {} +
make distclean
make x86 -j
make arm -j
make share
