
LIBS+= $(UNIT_TEST_LIBS)

ifeq (x86, $(TARGET))
	LIBS+= -lX11
endif

#export CPPFLAGS:=$(CPPFLAGS_RELAXED)
