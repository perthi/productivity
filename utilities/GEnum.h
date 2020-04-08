// -*- mode: c++ -*-

#ifndef KGENUM_H
#define KGENUM_H



/****************************************************
* Copyright: Embedded Consulting AS and others.
* For more information please contact pth@embc.no
******************************************************/



#include <string>
#include "GEnumHelper.h"

#define G_ENUM(E, ...) \
	enum class E __VA_ARGS__ ; \
	inline const char* operator*(E e) { return(GEnumHelper::Enum2ChPtr((int) e, #__VA_ARGS__ )); } \
	inline std::string operator&(E e) { return(GEnumHelper::Enum2Str((int) e, #__VA_ARGS__ )); } \
	inline static const char* Enum2ChPtr(E e) { return(GEnumHelper::Enum2ChPtr((int) e, #__VA_ARGS__ )); } \
	inline static std::string Enum2Str(E e) { return(GEnumHelper::Enum2Str((int) e, #__VA_ARGS__ )); } \
	inline static E EnumLookup(std::string lookupStr, E defaultVal, bool strictParse = false) { return((E)GEnumHelper::EnumLookup(lookupStr, (int)defaultVal, #__VA_ARGS__, strictParse )); }


#endif // !KGENUM_H
