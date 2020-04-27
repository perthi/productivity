
LIBNAME_A:=libtestlib.a
LIBNAME_SO:=libtestlib.so

SRCCPP+=TestBase.cpp


INCLUDES+=$(GTEST_INCLUDES)



include ../../common.mk
