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

#include "GTimeValidate.h"
#include "GTimeFormat.h"


#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include "GUtilities.h"
#include "GTokenizer.h"
#include "GNumbers.h"
#include "GRegexp.h"
#include <sstream>
#include <chrono>

#include <memory>
using namespace std::chrono;



GTimeValidate::GTimeValidate() :
    ValidDayFunct(    std::bind( &GTimeValidate::IsValidDay,    this,  std::placeholders::_1  ) ),
    ValidDateFunct(   std::bind( &GTimeValidate::IsValidDate,   this,  std::placeholders::_1  ) ),
    ValidMonthFunct(  std::bind( &GTimeValidate::IsValidMonth,  this,  std::placeholders::_1  ) ),
    ValidYearFunct(   std::bind( &GTimeValidate::IsValidYear,   this,  std::placeholders::_1  ) ),
    ValidHourFunct(   std::bind( &GTimeValidate::IsValidHour,   this,  std::placeholders::_1  ) ),
    ValidMinuteFunct( std::bind( &GTimeValidate::IsValidMinute, this,  std::placeholders::_1  ) ),
    ValidSecondFunct( std::bind( &GTimeValidate::IsValidSecond, this,  std::placeholders::_1  ) ),
    fTimeVal(0), fTimeFormat()
{

}




 /* Checks if a Year is a leap year.
  * @param[in] iYear
  * @return true if Leap Year othervise false */
bool GTimeValidate::IsGregorianLeapYear(int iYear)
{

    if (iYear % 4 != 0)
    {
        return false;
    }
    else if (iYear % 100 != 0)
    {
        return true;
    }
    else if (iYear % 400 != 0)
    {
        return false;
    }
    else
    {
        return true;
    }

}



bool GTimeValidate::IsValidDayS(string day)
{
    auto s_validDays = fTimeFormat.GetValidDays();
    return ValidateV2(   ValidDayFunct, day, &s_validDays );
}



bool  
GTimeValidate::IsValidDay(const int day)
{
    return ( day >= DAY_MIN && day <= DAY_MAX ) ? true : false;
}


bool 
GTimeValidate::IsValidDateS(const string date)
{
    return ValidateV2( ValidDateFunct , date);
}


bool 
GTimeValidate::IsValidDate(const int date)
{
    return ( date >= DATE_MIN && date <= DATE_MAX ) ? true : false; 
}




bool GTimeValidate::IsValidMonthS(const string month)
{
    auto  s_validMonths =  fTimeFormat.GetValidMonths();
    return ValidateV2(  ValidMonthFunct , month,  &s_validMonths );
}


bool  
GTimeValidate::IsValidMonth(const int month)
{
    return ( month >= MONTH_MIN && month <= MONTH_MAX) ? true : false;
}


bool 
GTimeValidate::IsValidYearS(const string year)
{
    return  ValidateV2( ValidYearFunct, year);
}


bool  GTimeValidate::IsValidYear(const int year)
{
    return ( year >= YEAR_MIN && year <= YEAR_MAX ) ? true : false;
}


bool GTimeValidate::IsValidHourS(const string hour)
{
    return ValidateV2( ValidHourFunct, hour);
}


bool  GTimeValidate::IsValidHour(const int hour)
{
    return  ( hour >= HOUR_MIN && hour <= HOUR_MAX ) ? true : false;
}


bool  
GTimeValidate::IsValidMinuteS(const string m)
{
    return ValidateV2( ValidMinuteFunct, m);
}


bool  
GTimeValidate::IsValidMinute(const int m)
{
    return  ( m >= MINUTE_MIN && m <= MINUTE_MAX) ? true : false;
}


bool  
GTimeValidate::IsValidSecond(const int s)
{
    return  ( (s >= SECONDS_MIN && s <= SECONDS_MAX) ? true : false );
}


bool GTimeValidate::IsValidSecondS(const string s)
{
    std::function<bool( const int )>  funct( std::bind( &GTimeValidate::IsValidSecond, this, std::placeholders::_1 ) );  
    return ValidateV2(  ValidSecondFunct, s); 
}


bool  
GTimeValidate::ValidateV2(  std::function<bool(  const int ) > funct, const string &s, const vector<string> * const arr  )
{
    if ( g_numbers()->IsInteger(s) == true)
    {
        int tmp = g_numbers()->ToInteger<int>(s);
         bool ret = funct(tmp);
        return    ret;
    }
    else
    {
        bool ret =  g_string()->BeginsWith(arr, s);
        return  ret; 
    }
}



bool 
GTimeValidate::IsValidFormat(const char *c, string &offender)
{
    if(c == nullptr)
    {
        return false;
    }

    static string last_valid = "";
    if(last_valid == string(c))
    {
        return true;
    }

    bool has_valid_token = false;
    bool has_invalid_token = false;

    vector<string> tokens = GTokenizer().Tokenize(c, "%");


    for (unsigned int i = 0; i < tokens.size(); i++)
    {
        bool isvalid = false;
        string tmp = "";
        if (tokens[i].size() >= 1)
        {
            tmp = "%" + tokens[i].substr(0, 1);
        }
        
       static  thread_local  map<string, string> s_format =  fTimeFormat.GetValidFormat();

        for (auto const item : s_format) 
        {
            if (item.first == tmp)
            {
                has_valid_token = true;
                isvalid = true;
            }
        }
        
        if (isvalid == false)
        {
            has_invalid_token = true;
            offender = tokens.at(i);
            break;
        }
        //continue;

    }

    if (has_invalid_token == true)
    {
        return false;
    }
    if (has_valid_token == true)
    {
     //   last_valid = string(c);
        return true;
    }
    return false;

}


