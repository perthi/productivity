
LIBNAME_SO:=  libsqlite-embc.so
LIBNAME_A:=   libsqlite-embc.a

#CFLAGS+=-fpermissive

CFLAGS+=-DSQLITE_ENABLE_JSON1  -fPIC


SRC+=sqlite3.c 


include ../../common.mk

