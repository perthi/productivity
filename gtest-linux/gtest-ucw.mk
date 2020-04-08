

LIBNAME_SO=libgtest.so
#LIBNAME_A=libgtest.a

SRCCPP:=  gtest-all.cpp

export CPPFLAGS:=$(CPPFLAGS_RELAXED)

include ../../common.mk


