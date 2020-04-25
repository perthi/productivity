// -*- mode: c++ -*-

#ifndef KGENUMHELPER_H
#define KGENUMHELPER_H


/****************************************************
* Copyright: Embedded Consulting AS and others.
* For more information please contact pth@embc.no
******************************************************/



#include "GDefinitions.h"
#include <string>
#include <map>

class GEnumHelper
{
public:
	static API const char* Enum2ChPtr(int val, const char* valStr);
	static API const std::string& Enum2Str(int val, const char* valStr);
	static API int EnumLookup(std::string lookupStr, int defaultVal, const char* valStr, bool strictParse);
};

#endif // !KGENUMHELPER_H
