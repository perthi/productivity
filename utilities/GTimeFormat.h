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
#include <ctime>
#include <time.h>
#include <map>
#include <vector>
#include <string>
#include <stdint.h>

using std::vector;
using std::string;
using std::map;



class GTimeFormat;

//GTimeFormat * g_time_format();


/** @brief various utility for access to system time which is can be availabel on a user specifed format
 *  @details Also contains function for leap-year checks and validity check for days, months etc.. */
class GTimeFormat 
{
  //  friend  GTimeFormat * g_time_format();
 
    
public:
    GTimeFormat();
    virtual ~GTimeFormat() {};

    vector<string>       API &  GetFormatSpecifiersV();
    map<string, string>  API &  GetValidFormat();
    vector<string>       API &  GetValidDays();  
    vector<string>       API &  GetValidMonths();
    string  str() ;


private:
    vector<string> s_validMonths;
    vector<string> s_validDays ;
    map<string, string> s_validFormat; 
    vector<string>  s_format_specifiers;
};





