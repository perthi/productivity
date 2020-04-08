#-------------------------------------------------
#
# Project created by QtCreator 2019-06-28T18:24:14
#
#-------------------------------------------------

QT       -= core gui

TARGET = src
TEMPLATE = lib

DEFINES += SRC_LIBRARY

TARGET=utilities

DESTDIR = ../../build/x86/lib

INCLUDEPATH+=${FERROTECH_HOME}/software/include


#include(../../modules.pri)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
GCommon.cpp \
GEnumHelper.cpp \
GFileIOHandler.cpp \
GMath.cpp \
GRandom.cpp \
GStackTrace.cpp \
GTokenizer.cpp \
GEnumHelperImpl.cpp \
GFileName_t.cpp \
GNumbers.cpp \
GRegexp.cpp \
GString.cpp \
GText.cpp \
GUtilities.cpp \
GDataTypes.cpp \
GException.cpp \
GLocation.cpp \
GPrintable.cpp \
GSystem.cpp \
GTime.cpp 


HEADERS += \
GColor.h \
GEnum.h \
GException.h \
GLocation.h \
GNumbers.h \
GRandom.h \
GStackTrace.h \
GTokenizer.h \
GCommon.h \
GDataTypes.h \
GEnumHelper.h \
GFileIOHandler.h \
GMath.h \
GParameter.h \
GRegexp.h \
GString.h \
GText.h \
GUtilities.h \
GConstants.h \
GDefinitions.h \
GEnumHelperImpl.h \
GFileName_t.h \
GMenu.h \
GPrintable.h \
GSystem.h \
GTime.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
