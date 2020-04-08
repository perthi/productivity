// -*- mode: c++ -*-

/****************************************************
* Copyright: Embedded Consulting AS and others.
* For more information please contact pth@embc.no
******************************************************/

#ifndef GENUMHELPERIMPL_H
#define GENUMHELPERIMPL_H

#include <string>
#include <map>
#include <vector>

class GEnumHelperElement
{
public:
	GEnumHelperElement(const char* valStr);
	const char* Enum2ChPtr(int val);
	const std::string& Enum2Str(int val);
	int EnumLookup(std::string lookupStr, int defaultVal, bool strictParse);
private:
	enum class eCHCLASS { LEFT_CURLY_BRACKET, WHITE_SPACE, TEXT, EQ, COMMA, RIGHT_CURLY_BRACKET };
	GEnumHelperElement::eCHCLASS getChClass(char ch);
	int ChPtr2Int(const char* p);
	std::string fDataStr;
	std::vector<int> fValues;
	std::vector<std::string> fStrings;
	std::map<int, std::string> fValMap;
};

class GEnumHelperImpl
{
public:
	GEnumHelperImpl() : fEnumMap() {}; 
	static GEnumHelperImpl* Instance();
	const char* Enum2ChPtr(int val, const char* valStr);
	const std::string& Enum2Str(int val, const char* valStr);
	int EnumLookup(std::string lookupStr, int defaultVal, const char* valStr, bool strictParse);
private:
	void SetupInstance(const char* valStr);
	std::map<std::string, GEnumHelperElement> fEnumMap;
};

#endif // !KGENUMHELPERIMPL_H
