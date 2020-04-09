

PROGRAM:=utilities-unit-tests

		

SRCCPP += utilities-dll-test.cpp \
	TestGMath.cpp \
	TestGRandom.cpp \
	TestGString.cpp \
	TestGText.cpp \
	TestGFileIOHandler.cpp  \
	TestGTokenizer.cpp \
	TestGRegexp.cpp \
	TestGSystem.cpp \
	TestGTime.cpp \
	TestGUtilities.cpp \
	TestGNumbers.cpp \
	TestDataTypesT.cpp \
	TestGEnum.cpp \
	TestGCrc.cpp


#TestGEnum.cpp 

include ../../../../common.mk
include ../../../../unittest-common.mk

LIBS+=  -lutilities -lreadline
INCLUDES+=$(GTEST_INCLUDES)
