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

#include "GTimeFormat.h"


#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include "GUtilities.h"
//#include "GTokenizer.h"
//#include "GNumbers.h"
//#include "GRegexp.h"
#include <sstream>
#include <chrono>
using namespace std::chrono;


/*
 * @class GTimeFormat class for 
 *  - manipulation of time/date stamps.
 *  - time estamp for the current time on user a defined format
 *  - Conversion between different forms of date representation
 *  
 *  The standard formattting used when converting to/from astring representation is shown below
 *     ***************************************************
 *     ***********    FORMAT SPECIFIERS      *************
 *     ***************************************************
 *     %a  =  abbrevated weekday
 *     %A  =  weekday
 *     %b  =  abbrevated month
 *     %B  =  month
 *     %C  =  Century (00-99)
 *     %d  =  Day of month (01-31), zero padded
 *     %e  =  Day of month ( 1-31), space padded
 *     %D  =  Equivalent to %m/%d/%y
 *     %F  =  Equivalent to %Y-%m-%d
 *     %g  =  last two digits of the week-based year
 *     %G  =  Week-based year
 *     %h  =  equivalent to %b (abbrevated month)
 *     %H  =  24-hour decimal (00-24)
 *     %I  =  12-hour decimal (00-12)
 *     %j  =  year day (001-366)
 *     %m  =  Month in decimal (01-12)
 *     %M  =  Minutes in decimal (00-59)
 *     %n  =  newline character
 *     %p  =  AM/PM designation
 *     %r  =  Equivalent to %I:%M : %S %p
 *     %R  =  Equivalent to %H:%M
 *     %S  =  Seconds in decimal (00-59)
 *     %t  =  newline character
 *     %T  =  Equivalent to  %H:%M:%S
 *     %u  =  week day in decimal (1-7)
 *     %w  =  week day in decimal (0-6)
 *     %U  =  sunday week number (00-53)
 *     %W  =  monday week number (00-53)
 *     %V  =  ISO 8601 week number (00-53)
 *     %X  =  Time display
 *     %y  =  year without century
 *     %Y  =  year with century"
 *     %z  =  Time zone
 *     %Z  =  Time zone name (if any)"
 *     %%  =  percent sign");
 */



// GTimeFormat * g_time_format()
// {
//     static  GTimeFormat *instance = new GTimeFormat();
//     return instance;
// }


GTimeFormat::GTimeFormat() : s_validMonths(), s_validDays(), 
                             s_validFormat(),s_format_specifiers()
{
s_validMonths = {
    "January", "February", "March",     "April",   "May",      "June",
    "July",    "August",   "September", "October", "November", "December"};

s_validDays = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun",
"monday",  "tuesday",  "wednesday",  "thursday", "friday", "saturday", "sunday"};

s_validFormat  = {
    {"%a", "abbrevated weekday"},
    {"%A", "weekday"},
    {"%b", "abbrevated month"},
    {"%B", "month"},
    {"%C", "Century (00-99)"},
    {"%d", "Day of month (01-31), zero padded"},
    {"%e", "Day of month ( 1-31), space padded"},
    {"%D", "Equivalent to \"%m/%d/%y\""},
    {"%F", "Equivalent to \"%Y-%m-%d\""},
    {"%g", "last two digits of the week-based year"},
    {"%G", "Week-based year"},
    {"%h", "equivalent to %b (abbrevated month)"},
    {"%H", "24-hour decimal (00-24)"},

    {"%I", "12-hour decimal (00-12)"},
    {"%j", "year day (001-366)"},
    {"%m", "Month in decimal (01-12)"},
    {"%M", "Minutes in decimal (00-59)"},

    {"%n", "newline character"},
    {"%p", "AM/PM designation"},
    {"%r", "Equivalent to \"%I:%M : %S %p\""},
    {"%R", "Equivalent to \"%H:%M\""},
    {"%S", "Seconds in decimal (00-59)"},
    {"%t", "newline character"},
    {"%T", "Equivalent to \"%H:%M:%S\""},
    {"%u", "week day in decimal (1-7)"},
    {"%w", "week day in decimal (0-6)"},

    {"%U", "sunday week number (00-53)"},
    {"%W", "monday week number (00-53)"},
    {"%V", " ISO 8601 week number (00-53)"},
    {"%X", "Time display"},

    {"%y", "year without century"},
    {"%Y", "year with century"},
    {"%z", "Time zone"},
    {"%z", "Time zone name (if any)"}};


    for (std::map<string, string>::iterator iter = s_validFormat.begin(); iter != s_validFormat.end(); ++iter)
    {
        s_format_specifiers.push_back(iter->first);
    }

}



vector<string>  &
GTimeFormat::GetFormatSpecifiersV()
{
    return  s_format_specifiers;

    // vector<string> tmp;
    // std::map<string, string>  format =  GetValidFormat();

    // for (std::map<string, string>::iterator iter = format.begin(); iter != format.end(); ++iter)
    // {
    //     tmp.push_back(iter->first);
    // }

    // return tmp;
}


map<string, string>   &
GTimeFormat::GetValidFormat()
{
    return s_validFormat;
}


vector<string>  &  
GTimeFormat::GetValidDays()
{
    return s_validDays;
}


vector<string>   &  
GTimeFormat::GetValidMonths()
{
    return   s_validMonths;

}


string
GTimeFormat::str() 
{
    string infostring = "******  Valid Format Specifiers  BEGIN ******\n";
    
    for (std::map<string, string>::iterator iter = s_validFormat.begin(); iter !=   s_validFormat.end(); ++iter)
    {
        infostring += iter->first + ":\t" + string(iter->second) + "\n";

    }

    infostring += "\n ***** Valid format specifiers END ******\n";
    return infostring;
}
