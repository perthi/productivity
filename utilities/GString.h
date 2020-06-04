// -*- mode: c++ -*-
#pragma once

/***************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                     ****
****************************************************************************/

/****************************************************************************
*** Copyright(C) 2018  Per Thomas Hille, pth@embc.no                      ***
*** This file is part of logmaster.logmaster is free software : you can   ***
*** redistribute it and / or modify it under the terms of the Lesser GNU  ***
*** General Public License(LGPL) V3 or later.See.cpp file for details     ***
*****************************************************************************/

#include "GDefinitions.h"
#include <string>
#include <cstring>


using std::string;
#include <vector>
using std::vector;

#include <stdio.h>
#include <algorithm>
#include <codecvt>
#include <sstream>
#include <iomanip>
#include <locale>



#define __STDC_WANT_LIB_EXT1__ 1
class GString;

 GString   * g_string();


/** @brief various string manipulation functions */
class GString
{
 friend  GString   * g_string();  

 public:
     char     *ReplaceBadChar(char *inputstring, const char delimeter = '_', const char *suspicious = 0);
     string    ReplaceBadChar(string &inputstring, const char delimeter = '_', const char *suspicious = 0);
     string    Replace(const string original, const string toreplace, const string replacement);
     
     string    ToPascalCase( const string  in ) const;
     string    ToUpperCase(  const string  in ) const;  
     string    ToLowerCase(  const string  in ) const;

     template <typename T>
     string    ToString(const vector<T> in, const string sep = "\n");
     
     template <typename T>
     string    ToString(const T in, const int w = -1, const char pad = '0');
     
     int          CountOccurences(const string input, const char c, const bool ignore_case);
     bool         Contains(const vector<string> &str, const string &substring, const bool ignore_case = true, long long int *pos = 0);
     bool         Contains(const string &str, const string &substring, const bool ignore_case = true, long long int *pos = 0);
     bool         BeginsWith(const string &str, const string &substring, const bool ignore_case = true);
     bool         BeginsWith(const vector<string> arr, const string token, const bool ignore_case = true);
     bool         EndsWith(const string &str, const string &substring, const bool ignore_case = true);
     bool         CompareNoCase(string lhs, string rhs) { return (ToUpper(lhs) == ToUpper(rhs)); }
     string       &Ltrim(string &s, const char c = ' ');
     string       &Rtrim(string &s, const char c = ' ');
     string       &Trim(string &s, const char c = ' ');
     string       &Trim(string &s, const vector<char> tokens);
     vector<string> Trim(vector<string> &s, const char c = ' ');
     vector<string> Trim(vector<string> &s, const vector<char> tokens);
     string       &Utf8ToAnsi(string &s);
     char         *Utf8ToAnsi(char *s);
     bool          IsAnsi(string &s);
     std::string   AnsiToUtf8(const std::string &ansiStr);
     string       &ToLower(string &s);
     string       &ToUpper(string &s);
     bool API      IsMatch(const string pattern, const string val, bool require_exact_match = false);
     string ReplaceBackSlash(const string input);
 private:
     GString(){};
     virtual ~GString(){};
};




template<typename T>
string
GString::ToString(const vector<T> in, const string sep)
{
    std::stringstream buffer;
	buffer.str("");

    for (uint16_t i = 0; i< in.size(); i++)
    {
		buffer << in[i] ;
		if (i < (in.size() - 1))
		{
			buffer << sep;
		}

	}
    return buffer.str();
}


template<typename T>
string
GString::ToString(const T in, const int w, const char pad) 
{
	std::stringstream buffer;
	buffer.str(std::string());

	if (w > 0)
    {
        buffer.width(w);
        buffer.fill(pad);
    }

    buffer << std::setprecision(std::numeric_limits<T>::max_digits10 ) << in;
    return buffer.str();
}


