

LIBNAME_SO=libgmock-embc.so
LIBNAME_A=libgmock-embc.a

#LIBNAME_SO=libgmock.so
#LIBNAME_A=libgmock.a

SRCCPP:=  gmock-all.cpp

#export CPPFLAGS:=$(CPPFLAGS_RELAXED)
export CPPFLAGS:=$(COMMON_FLAG)

#export INCLUDES+= $(INCLUDES_UNITTEST)
export INCLUDES=   -isystem . -isystem ../../

include ../../common.mk


