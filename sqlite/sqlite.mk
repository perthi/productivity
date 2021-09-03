
LIBNAME_SO:=  libsqlite-embc.so
LIBNAME_A:=   libsqlite-embc.a

#CFLAGS+=-fpermissive

CFLAGS+=-DSQLITE_ENABLE_JSON1  -DSQLITE_THREADSAFE=1 -fPIC -lpthread


SRC+=sqlite3.c 


include ../../common.mk

