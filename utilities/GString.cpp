// -*- mode: c++ -*-

/*****************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                       ****
******************************************************************************/

/*****************************************************************************
******************************************************************************
*** This file is part of logmaster.                                        ***
*** Copyright (C) 2018 Per Thomas Hille <pth@embc.no> http:///www.embc.no  ***
*** all rights reserved                                                    ***
***                                                                        ***
*** logmaster is free software: you can redistribute it and/or modify      ***
*** it under the terms of the Lesser GNU General Public License (LGPL)     ***
*** as published by the Free Software Foundation, either version 3 of the  ***
*** License, or (at your option) any later version.                        ***
***                                                                        ***
*** logmaster is distributed in the hope that it will be useful,           ***
*** but WITHOUT ANY WARRANTY; without even the implied warranty of         ***
*** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          ***
*** GNU General Public License for more details.                           ***
***                                                                        ***
*** You should have received a copy of the Lesser GNU General Public       ***
*** License along with logmaster in the README file in the source code     ***
*** directory. If not, see <http://www.gnu.org/licenses/>.                 ***
******************************************************************************
******************************************************************************/

#include "GString.h"

#include <algorithm>

GString  * g_string()
{
    static GString *instance = new  GString();
    return instance;
}



/** Count the number of times the character "c" occurs in the input string
*  @param[in] str  The input string to scan
*  @param[in] c    The character to count occurences of
*  @param[in]  ignore_case  Wether or not to ignore case when counting characters. If set to true, then both the input string
*  and the character will be converted to lower case before doing the comparison. Please not that this flag only applies to letters
*  A-Z for all other charcters the flag is ignored.
*  @return  The number of times "c" occurs in "str" */
int
GString::CountOccurences(const string str, const char c, const bool ignore_case)
{
    int cnt = 0;
    string l_str = str;
    char   l_c = c;

    if (ignore_case == true)
    {
        l_c = (char)tolower(l_c);
        l_str =  g_string()->ToLower(l_str);
    }
    for (unsigned int i = 0; i < l_str.size(); i++)
    {
        if (l_str[i] == l_c)
        {
            cnt++;
        }
    }
    return cnt;
}



/** Check if the string "str" the begins with "substring"
*   @param str The array to check for occurences of "token"
*   @param substring   The token to search for in "str"
*   @param ignore_case Wether or not to ignore case
*   @return true if "str" begins with "substring", false othervise" */
bool
GString::BeginsWith(const string &str, const string &substring, const bool ignore_case)
{
    long long int pos = 0;
    if (Contains(str, substring, ignore_case, &pos) == true && pos == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}



/** Check if any of the strings in the vector "arr"  begins with the token "token"
*   @param[in] arr The array to check for occurences of "token"
*   @param[in] token   The token to search for in the vector "arr"
*   @param[in] ignore_case wether or not the text search should be case sensitive.
*   @return true if "arr" has any element that begins with "token, false othervise" */
bool GString::BeginsWith(const vector<string> arr, const string token, const bool ignore_case)
{
    for (unsigned int i = 0; i < arr.size(); i++)
    {
        if (BeginsWith(arr[i], token, ignore_case) == true)
        {
            return true;
        }
    }
    return false;
}


 bool GString::EndsWith(const string& str, const string& substring, const bool ignore_case)
{
	long long int pos = 0;
	long long int expected_pos = (long long int)str.size() - (long long int)substring.size();
	if ((expected_pos >= 0) && Contains(str, substring, ignore_case, &pos) == true && (pos == expected_pos))
	{
		return true;
	}
	else
	{
		return false;
	}
}



/**@{
* Collection of functions to trim characters from a string
* *Ltrim*  trim characters from left.
* *Rtrim*  trim characters from right.
* *Ttrim*  trim characters from bot left and right.
* @param[in,out] s input string to trim
* @param[in] c  character to trim away / remove
* @return  The modified string */
string &
GString::Ltrim(string &s, const char c)
{
    size_t n_spaces = 0;
    size_t n = s.size();

    for (unsigned int i = 0; i < s.size(); i++)
    {
        if (s[i] == c)
        {
            n_spaces++;
        }
        else
        {
            break;
        }
    }
    s = s.substr(n_spaces, n);
    return s;
}


string &
GString::Rtrim(string &s, const char c)
{
    size_t n_spaces = 0;
    size_t n = s.size();

    for (size_t i = n; i > 0; i--)
    {
        if (s[i - 1] == c)
        {
            n_spaces++;
        }
        else
        {
            break;
        }
    }
    s = s.substr(0, n - n_spaces);
    return s;
}



vector<string>
GString::Trim(vector<string> &s, const char c)
{
    for (uint16_t i = 0; i < s.size(); i++)
    {
        Trim(s[i], c);
    }

    return s;
}


string &
GString::Trim(string &s, const char c)
{
    return Ltrim(Rtrim(s, c), c);
}
/**@}*/



/**@{
* @param[in,out] s input string to trim
* @param[in] tokens vector of tokens to trim away
* @return  The modified string */
string & GString::Trim(string & s, const vector<char> tokens)
{
    ///todo, PTH, this code segment is not very efficient
    string tmp;
    int64_t size_before = 0;
    int64_t size_after = 0;
    do
    {
        size_before = s.size();

        for (uint16_t i = 0; i < tokens.size(); i++)
        {

            Ltrim(Rtrim(s, tokens[i]), tokens[i]);
        }

        size_after = s.size();

    } while (size_before - size_after);

    return s;
}


vector<string> GString::Trim(vector<string>& s, const vector<char> tokens)
{
    for (uint16_t i = 0; i < s.size(); i++)
    {
        Trim(s[i], tokens);
    }
    return s;
}
/**@}*/



 /**  Converts string to lowercase. Handles both UTF-8 and ANSI, both will not convert ANSI to UTF-8.
 *  @param[in, out] s  String to be converted. The content is not altered.
 *  @return  lowercase of s. */
string& GString::ToLower(string& s)
{
    static string str;

    str = s;
    // IF UTF-8, convert to wide char and do to lower on wide char
    if (!IsAnsi(str))
    {
        std::locale loc("");

        std::wstring ws;
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        ws = converter.from_bytes(str);
        for (unsigned int n = 0; n < ws.size(); n++)
        {
            ws[n] = std::tolower(ws[n], loc);
        }
        str = converter.to_bytes(ws);
    }
    else
    {
        char* tmp = new char[str.length() + 1];
        memset(tmp, 0, (str.length() + 1));
        char *d = tmp;
        for (const char* s2 = str.c_str(); *s2; s2++)
        {
            *d++ = (char)tolower(*s2);
        }
        str = tmp;
        delete[] tmp;
    }
    return(str);
}




 /**  Converts string to uppercase. Handles both UTF-8 and ANSI, both will not convert ANSI to UTF-8.
  *  @param[in, out] s  String to be converted. The content is not altered.
  *  @return  uppercase of s.  */
string& GString::ToUpper(string& s)
{
    static string str;

    str = s;
    // IF UTF-8, convert to wide char and do to lower on wide char
    if (!IsAnsi(str))
    {
        std::locale loc("");

        std::wstring ws;
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        ws = converter.from_bytes(str);
        for (unsigned int n = 0; n < ws.size(); n++)
        {
            ws[n] = std::toupper(ws[n], loc);
        }
        str = converter.to_bytes(ws);
    }
    else
    {
        char* tmp = new char[str.length() + 1];
        memset(tmp, 0, (str.length() + 1));
        char *d = tmp;
        for (const char* s2 = str.c_str(); *s2; s2++)
        {
            *d++ = (char)toupper(*s2);
        }
        str = tmp;
        delete[] tmp;
    }
    return(str);
}








#define GET_SAFE_CHAR(str,ptr,len) (ptr<len?str[ptr]:'\0')


/**  Checks if a strins is Ansi/ASCII or UTF-8
*    @param[in, out] s  String to be check. The content is not altered.
*    @return  true if Ansi/ASCII. */
bool 
GString::IsAnsi(string& s)
{
	size_t len = s.size();
	const char* str = s.c_str();
	// IF UTF-8 BOM return false
	if ((GET_SAFE_CHAR(str, 0, len) == 0xEF) && (GET_SAFE_CHAR(str, 1, len) == 0xBB) && (GET_SAFE_CHAR(str, 2, len) == 0xBF))
	{
		return(false);
	}

	// OK, no BOM. Lets check for UTF-8 pattern......
	for (size_t n = 0; n < len; n++)
	{
		char ch = GET_SAFE_CHAR(str, n, len);
		if ((ch & 0xE0) == 0xC0)
		{
			char ch2 = GET_SAFE_CHAR(str, n + 1, len);
			if ((ch2 & 0xC0) == 0x80)
			{
				return(false);
			}
		}
		if ((ch & 0xF0) == 0xE0)
		{
			char ch2 = GET_SAFE_CHAR(str, n + 1, len);
			char ch3 = GET_SAFE_CHAR(str, n + 2, len);
			if (((ch2 & 0xC0) == 0x80) && ((ch3 & 0xC0) == 0x80))
			{
				return(false);
			}
		}
		if ((ch & 0xF8) == 0xF0)
		{
			char ch2 = GET_SAFE_CHAR(str, n + 1, len);
			char ch3 = GET_SAFE_CHAR(str, n + 2, len);
			char ch4 = GET_SAFE_CHAR(str, n + 3, len);
			if (((ch2 & 0xC0) == 0x80) && ((ch3 & 0xC0) == 0x80) && ((ch4 & 0xC0) == 0x80))
			{
				return(false);
			}
		}
	}

	return(true);
}



/**@{
 *  Convert UTF-8 encoded string to ANSI-encoded string
 *  @param[in, out] s  String to be converted. The content might be altered.
 *  @return  Copy of s  */
string& 
GString::Utf8ToAnsi(string& s)
{
    size_t memSize = s.size() + 1;
    char* s2 = new char[memSize];
    ::memset(s2, 0, memSize);
    ::memcpy(s2, s.c_str(), memSize);
    Utf8ToAnsi(s2);
    s = s2;
    delete[] s2;

    return(s);
}


char* 
GString::Utf8ToAnsi(char* s)
{

    char* srcStr = s;
    char* destStr = s;
    while (*srcStr)
    {
        if ((*srcStr & 0xD0) == 0xC0)
        {
            int ch1 = (*srcStr++) & 0x03;
            int ch2 = (*srcStr++) & 0x3F;
            int ch = (ch1 << 6) | ch2;
            if (ch > 0xFF)
            {
                ch = '?';
            }
            *destStr++ = (char)ch;
        }
        if ((*srcStr & 0xF0) == 0xE0)
        {
            *destStr++ = '?';
            srcStr += 3;
        }
        if ((*srcStr & 0xF8) == 0xF0)
        {
            *destStr++ = '?';
            srcStr += 4;
        }
        else
        {
            *destStr++ = *srcStr++;
        }
    }

    *destStr = '\0';

    return(s);
}
/**@}*/



/** Convert ANSI string to UTF8-encoded string
 *  @param[in] ansiStr  String to be converted.
 *  @return  utf8 string */
std::string
GString::AnsiToUtf8(const std::string& ansiStr)
{
    std::string utf8Str;
    utf8Str.resize(2 * ansiStr.size()); // worst case, all ansi bytes need 2 utf8 bytes

    for (size_t ansi_index = 0, utf8_index = 0; ansi_index < ansiStr.size(); ansi_index++) {
        if (static_cast<unsigned char>(ansiStr[ansi_index]) < 0x80) {
            utf8Str[utf8_index] = ansiStr[ansi_index];

            utf8_index += 1;
        }
        else {
            // implementation of: https://en.wikipedia.org/wiki/UTF-8
            const unsigned char byte1 = 0xC0 | (static_cast<unsigned char>(ansiStr[ansi_index]) >> 6);
            const unsigned char byte2 = 0x80 | (ansiStr[ansi_index] & 0x3F);

            utf8Str[utf8_index] = byte1;
            utf8Str[utf8_index + 1] = byte2;

            utf8_index += 2;
        }
    }

    utf8Str = std::string(utf8Str.c_str());

    return utf8Str;
}



bool  
GString::IsMatch(const string pattern, const string val, bool require_exact_match  ) 
{
    if (require_exact_match == true)
    {
        return pattern == val ? true : false;
    }
    else
    {
        return BeginsWith(val, pattern);
    }


    return false;
}

/**@{*/
/**   @brief Check if the string "str" contains "substring"
  *   @param[in] str The array to check for occurences of "token"
  *   @param[in] substring   The token to search for in "str"
  *   @param[in] ignore_case Wether or not to ignor case when comparing strings
  *   @param[out] pos  The position of the first occurence of "str"
  *   @return true if "str" contains "substring", false othervise" */
bool
GString::Contains(const vector<string>& str, const string& substring, const bool ignore_case, long long int* pos)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (Contains(str[i], substring, ignore_case, pos) == true)
		{
			return true;
		}
	}

	return false;

}


bool
GString::Contains(const string& str, const string& substring, const bool  ignore_case, long long int* pos)
{
	string l_str = str;
	string l_substr = substring;


	if (ignore_case == true)
	{
		std::transform(l_str.begin(), l_str.end(), l_str.begin(), ::tolower);
		std::transform(l_substr.begin(), l_substr.end(), l_substr.begin(), ::tolower);
	}


	unsigned long long int lpos = l_str.find(l_substr);

	if (pos != 0)
	{
		*pos = lpos;
	}

	if (lpos != std::string::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}
/**@}*/



/** Replace a substring of an input string with another substring
  * @param[in] original     The original string that one wants to make a modified copy of
  * @param[in] substring    The substring that one wants to replace
  * @param[in] replacement  The string that will replace "to replace"
  * @return    The modified string */
string
GString::Replace(const string original, const string substring, const string replacement)
{
	string tmp = original;
    if (replacement.size() == 0)
    {
        return tmp;
    }
    size_t start_pos = 0;
    
    while ((start_pos = tmp.find(substring, start_pos)) != std::string::npos) 
    {
        tmp.replace(start_pos, substring.length(), replacement  );
        start_pos += replacement.length(); // Handles case where 'to' is a substring of 'from'
    }
    return  tmp;
}


/**@{
 *  Replacing bad/unwanted characters in the input string, bad charcters ar characters such as for instance (,),{,},%# etc..
 *  @param[in,out] inputstring  The input string/character array that will be modified
 *  @param[in] delimeter  bad charcters will be replaced
 *  @param[in] suspicious  array of characters to be replaced, if it is a zero pointer then a default set of bad charcters will be used.
 *  @return  The modified string with all "bad" characters replaced with the delimeter. */
string
GString::ReplaceBadChar(string& inputstring, const char delimeter, const char* suspicious)
{
	static char tmp[1023];
	//SPRINTF(tmp, 1023, "%s", inputstring.c_str());
	SPRINTF_S(tmp, inputstring);
	ReplaceBadChar(tmp, delimeter, suspicious);
	inputstring = tmp;
	return inputstring;
}



char*
GString::ReplaceBadChar(char* inputstring, const char delimeter, const char* suspicious)
{
	static char souspiciouscharacters[1023];
	if (suspicious == 0)
	{
		SPRINTF_S(souspiciouscharacters, string(" \\,:()/{}#!*-^~?+=[]`??$&% "));
	}
	else
	{
		SPRINTF_S(souspiciouscharacters, string(suspicious));
	}


	long  long int length = strlen(inputstring);

	for (int i = 0; i < length; i++)
	{
		//  bool foundsuspicious = false;
		long long int badcharlength = strlen(souspiciouscharacters);
		for (int j = 0; j < badcharlength; j++)
		{
			if (inputstring[i] == souspiciouscharacters[j])
			{
				inputstring[i] = delimeter;
				//      foundsuspicious = true;
			}
		}
	}

	static char out[1023];
	SPRINTF_S(out, string(inputstring));
	return out;
}
/**@}*/


 string    
 GString::ToPascalCase(const string in) const
 {
    if(in.size( ) >  1 )
    {
      
      string copy = in;
      std::transform( copy.begin() +1, copy.end(),  copy.begin() +1, ::tolower );           
      std::transform( copy.begin(),    copy.begin() + 1,  copy.begin(), ::toupper );        
      return copy;    
    }
    else
    {
        return in;
    }
 }


string    
GString::ToUpperCase(const string in) const
{
   string copy = in;  
   std::transform( copy.begin(), copy.end(),  copy.begin(), ::toupper );    
   return copy;
}

string    
GString::ToLowerCase(const string in) const
{
    
     string copy = in;  
     std::transform( copy.begin(), copy.end(),  copy.begin(), ::tolower );    
     return copy;   
}



string
GString::ReplaceBackSlash(const string /*input*/ )
{
	string out = ""; 

/*
	
	for (auto c : input)
	{
		switch (c)
		{
		case '\a':
			out += "\\a";
			break;
		case '\b':
			out + "\\b";
			break;
		case '\c':
			out + "\\c";
			break;
		case '\d':
			out + "\\d";
			break;
		case '\e':
			out + "\\e";
			break;
		case '\f':
			out + "\\f";
			break;
		case '\g':
			out + "\\g";
			break;
		case '\h':
			out + "\\h";
			break;
		case '\i':
			out + "\\i";
			break;
		case '\j':
			out + "\\j";
			break;
		case '\k':
			out + "\\k";
			break;
		case '\l':
			out + "\\l";
			break;
		case '\m':
			out + "\\m";
			break;
		case '\n':
			out + "\\n";
			break;
		case '\o':
			out + "\\o";
			break;
		case '\p':
			out + "\\p";
			break;
		case '\q':
			out + "\\q";
			break;
		case '\r':
			out + "\\r";
			break;
		case '\s':
			out + "\\s";
			break;
		case '\t':
			out + "\\t";
			break;
		case '\u':
			out + "\\u";
			break;
		case '\v':
			out + "\\v";
			break;
		case '\w':
			out + "\\w";
			break;
		case '\x':
			out + "\\x";
			break;
		case '\y':
			out + "\\y";
			break;
		case '\z':
			out + "\\z";
			break;
		case '\A':
			out + "\\A";
			break;
		case '\B':
			out + "\\B";
			break;
		case '\C':
			out + "\\C";
			break;
		case '\D':
			out + "\\D";
			break;
		case '\E':
			out + "\\E";
			break;
		case '\F':
			out + "\\F";
			break;
		case '\G':
			out + "\\G";
			break;
		case '\H':
			out + "\\H";
			break;
		case '\I':
			out + "\\I";
			break;
		case '\J':
			out + "\\J";
			break;
		case '\K':
			out + "\\K";
			break;
		case '\L':
			out + "\\L";
			break;
		case '\M':
			out + "\\M";
			break;
		case '\N':
			out + "\\N";
			break;
		case '\O':
			out + "\\O";
			break;
		case '\P':
			out + "\\P";
			break;
		case '\Q':
			out + "\\Q";
			break;
		case '\R':
			out + "\\R";
			break;
		case '\S':
			out + "\\S";
			break;
		case '\T':
			out + "\\T";
			break;
		case '\U':
			out + "\\U";
			break;
		case '\V':
			out + "\\V";
			break;
		case '\W':
			out + "\\W";
			break;
		case '\X':
			out + "\\X";
			break;
		case '\Y':
			out + "\\Y";
			break;
		case '\Z':
			out + "\\Z";
			break;
		case '\0':
			out + "\\0";
			break;
		case '\1':
			out + "\\1";
			break;
		case '\2':
			out + "\\2";
			break;
		case '\3':
			out + "\\3";
			break;
		case '\4':
			out + "\\4";
			break;
		case '\5':
			out + "\\5";
			break;
		case '\6':
			out + "\\6";
			break;
		case '\7':
			out + "\\7";
			break;
		case '\8':
			out += "\\8";
			break;
		case '\9':
			out += "\\9";
			break;

		default:
			out.push_back(c);
			break;

		}
	}
*/

	return out;
}
