// -*- mode: c++ -*-

#ifndef PREGEXPXX_H
#define PREGEXPXX_H


/***************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                     ****
****************************************************************************/

/****************************************************************************
*** Copyright(C) 2018  Per Thomas Hille, pth@embc.no                      ***
*** This file is part of logmaster.logmaster is free software : you can   ***
*** redistribute it and / or modify it under the terms of the Lesser GNU  ***
*** General Public License(LGPL) V3 or later.See.cpp file for details     ***
*****************************************************************************/

#include "GNumbers.h"
#include <string>
#include <vector>
using std::vector;
using std::string;

#include <regex>


class GRegexp;

inline  GRegexp * g_regexp();

/** @brief Parsing of strings using simplified regular expressions, or just plain strings */
class  GRegexp
{
public:
	API GRegexp() {};
	API ~GRegexp() {};

    inline  vector <string>  API  ScanNumber ( const string fname, const string digits = "1,99");
	inline  bool				 IsMatch(const string input, const std::regex e);
	inline bool				 IsMatch(const string input, const string to_match);
	inline vector<string>	 API  Filter( const vector<string> input, const std::regex e, vector<string> *removed = 0);

private:
    inline bool CheckFormat(const string format);
};



GRegexp * g_regexp()
{
	static GRegexp *instance = new GRegexp();
	return instance;
}



#undef A
#undef B
#undef P
#undef D
#undef T
#undef U
#undef V



/*
#ifndef ARM
#include <boost/regex.hpp>
using boost::regex;
#else
#include <regex>
#endif
*/

#include <regex>

/** @brief Scans number from a string
*   @param input the input string to scan
*   @param digits the number of digits required (written on string format for convinence since it is used directly in the
*   regular expression). For instance if digist = "4" the it is searched only for four digit numbers (for instance 1234) etc,.. whereas
*   digits = "1,3" searches for number with 1,2 or 3 digits
*   @return a vector of numbers written out on string format.
*   @throw A "boost::exception_detail" if the regular exression is invalid
*   @todo This function should be moved to one of the utilities classes.
*   @bug It must be checked that "digits" is actually a number */
vector <string>
GRegexp::ScanNumber(const string input, const string digits)
{
    if (CheckFormat(digits) == false)
    {
        throw(std::invalid_argument(("Invalid format string: " + digits + ",  The format string must be  a single digit, or two digits separated by a comma, e.eg  3,6").c_str()));
    }
    string expr;

    //#ifndef ARM
    std::regex e("[0-9]{" + digits + "}", regex_constants::ECMAScript);
    sregex_token_iterator iter(input.begin(), input.end(), e, 0);
    sregex_token_iterator end;

    //#endif

    vector<string> matches;

    int i = 0;
    for (; iter != end; ++iter)
    {
        //COUT << "iter[" << i << "] = " << *iter << "\tinput = "<< input << endl; 
        matches.push_back(*iter);
        i++;
    }
    return matches;
//    return vector<string>();
}


/*  Checks wether or not a string matches a pattern wher pattern must be a valid regular expression. It is possible to
*  use wildcars (*). For instance "lorem ipsum" will match the patther "(.*)psum"
*  and the pattern lor(.*) or the pattern (.*)em ips(.*). Furthermore the patern l(.*)sum wil also match "lorem ipsum".
*	If the pattern string does not contain any wildcards then an exact macth between the input string and the pattern is
*  required.
*  @param input[in] The input string to check
*  @param pattern[in] The pattern to match the input string against
*  @return true if the input matches pattern, false othervise
*  @exception std::regex_error if the regular expression is not well formed*/
bool
GRegexp::IsMatch(const string input, const std::regex e)
{
    std::cmatch cm;
    std::regex_match(input.c_str(), cm, e);

    if (cm.str() == "")
    {
        return false;
    }
    else
    {
        return true;
    }
}


bool
GRegexp::IsMatch(const string input, const string to_match)
{
    string expression = g_string()->Replace(g_string()->Replace(g_string()->Replace(to_match, "*", "(.*)"), "[", "\\["), "]", "\\]");
    return IsMatch(input, std::regex(expression));
}



/* Filtering a vector of string using a single pattern
* @param input[in] The input vector to filter
* @param pattern[in] The pattern to match against
* @param removed[in|out]  The vector of entries that was removed
* @return the resulting vector after the filter has been applied*/
vector<string>
GRegexp::Filter(const vector<string> input, const std::regex e, vector<string>* removed)
{
    vector<string> filtered_in;

    for (uint16_t i = 0; i < input.size(); i++)
    {
        if (IsMatch(input[i], e))
        {
            filtered_in.push_back(input[i]);
        }
        else
        {
            if (removed != 0)
            {
                removed->push_back(input[i]);
            }
        }
    }
    return filtered_in;
}



/* Helper function that validates the format specifier when scanning numbers from a string
*  @param format The format string to validate
*  @return true if "format" is on the form "a,b"  where a and b are positive integers, false othervise */
bool
GRegexp::CheckFormat(const string format)
{
    for (unsigned int i = 0; i < format.size(); i++)
    {
        if (!g_numbers()->IsInteger(format.at(i)) && format.at(i) != ',' &&   format.at(i) != ' ')
        {
            return false;
        }

        if (g_numbers()->IsInteger(format.at(i)) == true)
        {
            int num = g_numbers()->ToInteger<int>(g_string()->ToString(format.at(i)));
            if (num < 0)
            {
                throw(std::invalid_argument("The number of digits specified must be positive, " + format + ", is not all positive numbers "));
            }
        }
    }
    return true;
}





#endif
