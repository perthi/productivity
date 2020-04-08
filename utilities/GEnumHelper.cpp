// -*- mode: c++ -*-

#include "GEnumHelper.h"
#include "GEnumHelperImpl.h"



/****************************************************
* Copyright: Embedded Consulting AS and others.
* For more information please contact pth@embc.no
******************************************************/



const char* GEnumHelper::Enum2ChPtr(int val, const char* valStr)
{
	return( GEnumHelperImpl::Instance()->Enum2ChPtr(val, valStr));
}

const std::string& GEnumHelper::Enum2Str(int val, const char* valStr)
{
	return( GEnumHelperImpl::Instance()->Enum2Str(val, valStr));
}

int GEnumHelper::EnumLookup(std::string lookupStr, int defaultVal, const char* valStr, bool strictParse)
{
	return( GEnumHelperImpl::Instance()->EnumLookup(lookupStr, defaultVal, valStr, strictParse));
}
