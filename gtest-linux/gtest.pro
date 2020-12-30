#-------------------------------------------------
#
# Project created by QtCreator 2019-06-28T18:37:38
#
#-------------------------------------------------

QT       -= core gui

TARGET = gtest-embc
TEMPLATE = lib

DEFINES += SRC_LIBRARY

DESTDIR = ../build/x86/lib



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
gtest-all.cpp \
gtest.cpp \
gtest-death-test.cpp \
gtest-filepath.cpp \
gtest_main.cpp \
gtest-port.cpp \
gtest-printers.cpp \
gtest-test-part.cpp \
gtest-typed-test.cpp


HEADERS += \
gtest-all.h \
gtest-filepath.h \
gtest-internal-inl.h \
gtest-param-test.h \
gtest-port-arch.h \
gtest-printers.h \
gtest-string.h \
gtest-typed-test.h \
gtest-death-test.h \
gtest.h \
gtest-linked_ptr.h \
gtest-param-util-generated.h \
gtest-port.h \
gtest_prod.h \
gtest-test-part.h \
gtest-type-util.h \
gtest-death-test-internal.h \
gtest-internal.h \
gtest-message.h \
gtest-param-util.h \
gtest_pred_impl.h \
gtest-spi.h \
gtest-tuple.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
