

##ifneq (ARM, $(TARGET))
## LIBNAME_SO:=libtestlib.so
##endif

#LIBNAME_A:=libtestlib.a
LIBNAME_SO:=libtestlib.so

SRCCPP+=TestBase.cpp


INCLUDES+=$(GTEST_INCLUDES)

export CPPFLAGS:=$(CPPFLAGS_RELAXED)

include ../../common.mk
