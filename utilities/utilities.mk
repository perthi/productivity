


## ifneq (ARM, $(TARGET))
LIBNAME_SO:=libutilities.so
## endif

LIBNAME_A:=libutilities.a


SRCCPP+=GCommon.cpp \
	GFileIOHandler.cpp \
	GDataTypes.cpp \
	GFileName_t.cpp \
	GLocation.cpp \
	GMath.cpp \
	GNumbers.cpp \
	GPrintable.cpp \
	GRandom.cpp \
	GRegexp.cpp \
	GString.cpp \
	GText.cpp \
	GSystem.cpp \
	GTime.cpp \
	GTimeFormat.cpp \
	GTimeValidate.cpp \
	GTimeSpan.cpp \
	GTokenizer.cpp \
	GUtilities.cpp \
	GEnumHelper.cpp \
	GEnumHelperImpl.cpp \
	GSemaphore.cpp \
	GCrc.cpp \
	GStackTrace.cpp \
	GDataBaseIF.cpp \
	GFormatting.cpp


include ../../common.mk
