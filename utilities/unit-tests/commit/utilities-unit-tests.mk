

PROGRAM:=utilities-unit-tests


SRCCPP += utilities-dll-test.cpp \
	TestGMath.cpp \
	TestGRandom.cpp \
	TestGString.cpp \
	TestGText.cpp \
	TestGFileIOHandler.cpp  \
	TestGTokenizer.cpp \
	TestGRegexp.cpp \
	TestGTime.cpp \
	TestGUtilities.cpp \
	TestGNumbers.cpp \
	TestGEnum.cpp \
	TestDataTypesT.cpp  \
    TestGCrc.cpp \
	TestGSystem.cpp



include ../../../../common.mk
include ../../../../unittest-common.mk

LIBS+=  -lutilities -lreadline
INCLUDES+=$(GTEST_INCLUDES)
