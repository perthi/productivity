TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt



SOURCES += \
utilities-dll-test.cpp \
TestDataTypesT.cpp \
TestGMath.cpp \
TestGRandom.cpp \
TestGString.cpp \
TestGText.cpp \
TestGTokenizer.cpp \
TestException.cpp \
TestGFileIOHandler.cpp \
TestGNumbers.cpp \
TestGRegexp.cpp \
TestGSystem.cpp \
TestGTime.cpp \
TestGUtilities.cpp


HEADERS += \
utilities-dll-test.h \
TestException.h \
TestGFileIOHandler.h \
TestGNumbers.h \
TestGRegexp.h \
TestGSystem.h \
TestGTime.h \
TestGUtilities.h \
TestDataTypesT.h \
TestGMath.h \
TestGRandom.h \
TestGString.h \
TestGText.h \
TestGTokenizer.h

TARGET=utilities-unit-tests

DESTDIR = ../../../../build/x86/bin


INCLUDEPATH+=${FERROTECH_HOME}/software/include
INCLUDEPATH+= -isystem ${FERROTECH_HOME}/software
LIBS += -L${FERROTECH_HOME}/software/build/x86/lib


LIBS+=  -llogmaster -ltestlib  -lutilities -lcmdline -lgtest  -lpthread -lboost_system -lboost_filesystem

