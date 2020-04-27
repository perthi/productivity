

PROGRAM:=utilities-unit-tests


SRCCPP += utilities-unit-tests.cpp \
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

LIBS+=   -lutilities  -lreadline

ifdef HAS_LOGGING
LIBS+=  -lexception -llogmaster	
endif

INCLUDES+=$(GTEST_INCLUDES)
