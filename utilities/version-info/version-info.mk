
PROGRAM:= version-info
LIBNAME_SO:= libversion-info.so

SRCCPP+=	version-info.cpp \
		GGenerateVersionInfo.cpp


LIBS+= -lexception  -llogmaster -lutilities  -llogmaster   -lexception -lutilities -lcmdline -lsqlite-embc  -ldl -lpthread -lreadline -lhistory -lncurses

include ../../../common.mk
