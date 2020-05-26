

LIBNAME_SO=libgtest.so
LIBNAME_A=libgtest.a

SRCCPP:=  gtest-all.cpp

#export CPPFLAGS:=$(CPPFLAGS_RELAXED)

export CPPFLAGS:=  -fPIC -std=c++17  -g 

include ../../common.mk


