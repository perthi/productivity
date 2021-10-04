

LIBNAME_SO=libgtest-embc.so
LIBNAME_A=libgtest-embc.a

#LIBNAME_SO=libgtest.so
#LIBNAME_A=libgtest.a

SRCCPP:=  gtest-all.cpp

#export CPPFLAGS:=$(CPPFLAGS_RELAXED)
export CPPFLAGS:=$(COMMON_FLAGS)

# export INCLUDES+= $(INCLUDES_UNITTEST)
export INCLUDES=   -isystem .  -isystem ../../

include ../../common.mk


