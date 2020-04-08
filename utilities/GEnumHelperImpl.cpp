// -*- mode: c++ -*-

/****************************************************
* Copyright: Embedded Consulting AS and others.
* For more information please contact pth@embc.no
******************************************************/


#include "GEnumHelperImpl.h"
#include <cstring>


GEnumHelperElement::GEnumHelperElement(const char* valStr)	: fDataStr(""), fValues(), fStrings(), fValMap()
{
	fDataStr = valStr;

	enum class eSTATE { START, NAMESTART, NAME, NAMEEND, STARTVAL, VAL, VALEND, END } state = eSTATE::START;
	size_t slen = fDataStr.length() + 1;
	char* str = new char[slen];

	#ifdef _WIN32
	strcpy_s(str, slen, fDataStr.c_str());
	#else
	strncpy(str, fDataStr.c_str(), slen );
	#endif

	char* p = str;
	int elements = 1;
	while (*p != '\0')
	{
		if (*p++ == ',') elements += 1;
	}
	fValues.resize(elements);
	fStrings.resize(elements);
	char* textStartPtr = str;
	int element = 0;
	int elementValue = 0;
	for (p = str; *p != '\0'; p++)
	{
		switch (state)
		{
		case eSTATE::START:
			switch (getChClass(*p))
			{
				case eCHCLASS::LEFT_CURLY_BRACKET:
					state = eSTATE::NAMESTART;
					textStartPtr = p + 1;
					break;
				case eCHCLASS::WHITE_SPACE:
				case eCHCLASS::TEXT:
				case eCHCLASS::EQ:
				case eCHCLASS::COMMA:
				case eCHCLASS::RIGHT_CURLY_BRACKET:
				default:
					break;
			}
			break;
		case eSTATE::NAMESTART:
			switch (getChClass(*p))
			{
				case eCHCLASS::LEFT_CURLY_BRACKET:
					state = eSTATE::NAME;
					break;
				case eCHCLASS::WHITE_SPACE:
					textStartPtr = p + 1;
					break;
				case eCHCLASS::TEXT:
					state = eSTATE::NAME;
					break;
				case eCHCLASS::EQ:
				case eCHCLASS::COMMA:
				case eCHCLASS::RIGHT_CURLY_BRACKET:
				default:
					state = eSTATE::END;
					break;
			}
			break;
		case eSTATE::NAME:
			switch (getChClass(*p))
			{
				case eCHCLASS::LEFT_CURLY_BRACKET:
					state = eSTATE::END;
					break;
				case eCHCLASS::WHITE_SPACE:
					*p = '\0';
					state = eSTATE::NAMEEND;
					break;
				case eCHCLASS::TEXT:
					state = eSTATE::NAME;
					break;
				case eCHCLASS::EQ:
					*p = '\0';
					fStrings[element] = textStartPtr;
					state = eSTATE::STARTVAL;
					textStartPtr = p + 1;
					break;
				case eCHCLASS::COMMA:
					*p = '\0';
					fStrings[element] = textStartPtr;
					fValues[element++] = elementValue++;
					state = eSTATE::NAMESTART;
					textStartPtr = p + 1;
					break;
				case eCHCLASS::RIGHT_CURLY_BRACKET:
					*p = '\0';
					fValues[element++] = elementValue++;
					state = eSTATE::END;
					break;
				default:
					state = eSTATE::END;
					break;
			}
			break;
		case eSTATE::NAMEEND:
			switch (getChClass(*p))
			{
				case eCHCLASS::LEFT_CURLY_BRACKET:
					state = eSTATE::END;
					break;
				case eCHCLASS::WHITE_SPACE:
					break;
				case eCHCLASS::TEXT:
					state = eSTATE::END;
					break;
				case eCHCLASS::EQ:
					*p = '\0';
					fStrings[element] = textStartPtr;
					state = eSTATE::STARTVAL;
					textStartPtr = p + 1;
					break;
				case eCHCLASS::COMMA:
					*p = '\0';
					fStrings[element] = textStartPtr;
					fValues[element++] = elementValue++;
					state = eSTATE::NAMESTART;
					textStartPtr = p + 1;
					break;
				case eCHCLASS::RIGHT_CURLY_BRACKET:
					*p = '\0';
					fValues[element++] = elementValue++;
					state = eSTATE::END;
					break;
				default:
					state = eSTATE::END;
					break;
			}
			break;
		case eSTATE::STARTVAL:
			switch (getChClass(*p))
			{
				case eCHCLASS::LEFT_CURLY_BRACKET:
					state = eSTATE::END;
					break;
				case eCHCLASS::WHITE_SPACE:
					textStartPtr = p + 1;
					break;
				case eCHCLASS::TEXT:
					state = eSTATE::VAL;
					break;
				case eCHCLASS::EQ:
					state = eSTATE::END;
					break;
				case eCHCLASS::COMMA:
					state = eSTATE::END;
					break;
				case eCHCLASS::RIGHT_CURLY_BRACKET:
					state = eSTATE::END;
					break;
				default:
					state = eSTATE::END;
					break;
			}
			break;
		case eSTATE::VAL:
			switch (getChClass(*p))
			{
				case eCHCLASS::LEFT_CURLY_BRACKET:
					state = eSTATE::END;
					break;
				case eCHCLASS::WHITE_SPACE:
					*p = '\0';
					state = eSTATE::VALEND;
					break;
				case eCHCLASS::TEXT:
					state = eSTATE::VAL;
					break;
				case eCHCLASS::EQ:
					state = eSTATE::END;
					break;
				case eCHCLASS::COMMA:
					*p = '\0';
					elementValue = ChPtr2Int(textStartPtr);
					fValues[element++] = elementValue++;
					state = eSTATE::NAMESTART;
					textStartPtr = p + 1;
					break;
				case eCHCLASS::RIGHT_CURLY_BRACKET:
					*p = '\0';
					elementValue = ChPtr2Int(textStartPtr);
					fValues[element++] = elementValue++;
					state = eSTATE::END;
					break;
				default:
					state = eSTATE::END;
					break;
			}
			break;
		case eSTATE::VALEND:
			switch (getChClass(*p))
			{
				case eCHCLASS::LEFT_CURLY_BRACKET:
					state = eSTATE::END;
					break;
				case eCHCLASS::WHITE_SPACE:
					state = eSTATE::VALEND;
					break;
				case eCHCLASS::TEXT:
					state = eSTATE::END;
					break;
				case eCHCLASS::EQ:
					state = eSTATE::END;
					break;
				case eCHCLASS::COMMA:
					*p = '\0';
					elementValue = ChPtr2Int(textStartPtr);
					fValues[element++] = elementValue++;
					state = eSTATE::NAMESTART;
					textStartPtr = p + 1;
					break;
				case eCHCLASS::RIGHT_CURLY_BRACKET:
					*p = '\0';
					elementValue = ChPtr2Int(textStartPtr);
					fValues[element++] = elementValue++;
					state = eSTATE::END;
					break;
				default:
					state = eSTATE::END;
					break;
			}
			break;
		case eSTATE::END:
			break;
		}
	}

	delete[] str;

	for (int n = 0; n < elements; n++)
	{
		fValMap.emplace(fValues[n], fStrings[n]);
	}
}

const char* GEnumHelperElement::Enum2ChPtr(int val)
{
	return(fValMap.at(val).c_str());
}

const std::string& GEnumHelperElement::Enum2Str(int val)
{
	return(fValMap.at(val));
}

int GEnumHelperElement::EnumLookup(std::string lookupStr, int defaultVal, bool strictParse)
{
	for (auto it = fValMap.begin(); it != fValMap.end(); it++)
	{
		if (strictParse)
		{
			if (it->second == lookupStr)
			{
				return(it->first);
			}
		}
		else
		{
			#ifdef _WIN32
			if (::_strcmpi( it->second.c_str(), lookupStr.c_str()) == 0)
			#else
			if ( strcmp( it->second.c_str(), lookupStr.c_str()) == 0)
			#endif
			{
				return(it->first);
			}
		}
	}
	return (defaultVal);
}

GEnumHelperElement::eCHCLASS GEnumHelperElement::getChClass(char ch)
{
	if (ch == '{')
	{
		return(eCHCLASS::LEFT_CURLY_BRACKET);
	}
	else if (ch <= ' ')
	{
		return(eCHCLASS::WHITE_SPACE);
	}
	else if (ch == '=')
	{
		return(eCHCLASS::EQ);
	}
	else if (ch == ',')
	{
		return(eCHCLASS::COMMA);
	}
	else if (ch == '}')
	{
		return(eCHCLASS::RIGHT_CURLY_BRACKET);
	}
	else
	{
		return (eCHCLASS::TEXT);
	}
}

int GEnumHelperElement::ChPtr2Int(const char* p)
{
	if (*p == '0')
	{
		if ((*(p + 1) == 'x') || (*(p + 1) == 'X'))
		{
			char* end;
			int res = ::strtol(p + 2, &end, 16);
			return(res);
		}
		else
		{
			char* end;
			int res = ::strtol(p + 1, &end, 8);
			return(res);
		}
	}
	return (::atoi(p));
}

GEnumHelperImpl* GEnumHelperImpl::Instance()
{
	static GEnumHelperImpl* instance = nullptr;

	if (instance == nullptr)
	{
		instance = new GEnumHelperImpl();
	}

	return(instance);
}

const char* GEnumHelperImpl::Enum2ChPtr(int val, const char* valStr)
{
	SetupInstance(valStr);
	return(fEnumMap.at(valStr).Enum2ChPtr(val));
}

const std::string& GEnumHelperImpl::Enum2Str(int val, const char* valStr)
{
	SetupInstance(valStr);
	return(fEnumMap.at(valStr).Enum2Str(val));
}

int GEnumHelperImpl::EnumLookup(std::string lookupStr, int defaultVal, const char* valStr, bool strictParse)
{
   // FORCE_DEBUG("lookupstr = %s", lookupStr.c_str()   );
    
    SetupInstance(valStr);
    return(fEnumMap.at(valStr).EnumLookup(lookupStr, defaultVal, strictParse));
}

void GEnumHelperImpl::SetupInstance(const char * valStr)
{
	if (fEnumMap.find(valStr) == fEnumMap.end())
	{
		fEnumMap.emplace(valStr, GEnumHelperElement(valStr));
	}
}
