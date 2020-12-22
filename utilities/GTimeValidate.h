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
#include "GTimeFormat.h"
#include <ctime>
#include <time.h>
#include <map>
#include <vector>
#include <string>
#include <stdint.h>
#include <functional>

using std::vector;
using std::string;
using std::map;



/** @brief various utility for acess to system time which is can be availabel on a user specifed format
 *  @details Also contains function for leap-year checks and validtiy check for days, months etc.. */
class GTimeValidate 
{
public:
    GTimeValidate();
    ~GTimeValidate() {};
  
    bool			API   IsGregorianLeapYear(int iYear);
    
    bool			API   IsValidDayS(    string  day );
    bool			API   IsValidDay(   const int  day);
    std::function<bool( const int )>    ValidDayFunct; 

    bool			API   IsValidDateS(  const string  date); 
    bool			API   IsValidDate(  const int  date);
    std::function<bool( const int )>    ValidDateFunct; 

    bool			API   IsValidMonthS( const string  month);
    bool			API   IsValidMonth( const int     month);
    std::function<bool( const int )>    ValidMonthFunct; 

    bool			API   IsValidYearS(  const string  year);
    bool			API   IsValidYear(  const int     year);
    std::function<bool( const int )>    ValidYearFunct; 

    bool			API   IsValidHourS(  const string  hour);
    bool			API   IsValidHour(  const int     hour);
    std::function<bool( const int )>    ValidHourFunct; 

    bool			API   IsValidMinuteS( const string  m);
    bool			API   IsValidMinute( const int     h);
    std::function<bool( const int )>    ValidMinuteFunct; 

    bool			API   IsValidSecondS( const string  s);
    bool			API   IsValidSecond( const int     s);
    std::function<bool( const int )>    ValidSecondFunct; 

    bool			API   IsValidFormat( const char  *c,  string & offender);
    

    
#ifdef _WIN32
	static GTimeValidate API Now();
#endif
  

private:
    bool  ValidateV2(  std::function<bool(  const int ) > funct, const string &s, const vector<string> * const arr = nullptr );
    int64_t fTimeVal;
    GTimeFormat    fTimeFormat;
};





