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
#include "GTimeValidate.h"
#include "GTimeSpan.h"
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

#define MAX_TIMESTAMP_SIZE 256

/** @brief various utility for acess to system time which is can be availabel on a user specifed format
 *  @details Also contains function for leap-year checks and validtiy check for days, months etc.. */
class GTime 
{
public:
    GTime();
	API GTime(int year, int month, int dayInMonth, int hour24 = 0, int minute = 0, int second = 0, int us = 0);
    ~GTime() {};
    


    void            API   SetTime(int year, int month, int dayInMonth, int hour24 = 0, int minute = 0, int second = 0, int us = 0);
    long double		API   AccessDate();
    long double		API   AccessDate(const string date);
    string			API   TimeStamp(struct std::tm *tout = 0, const char * format = 0, struct std::tm *tin = 0, int64_t *us = 0);
    string			API   TimeStamp(const char * format, int64_t *us = 0 );
    
    double          API   GetEpochTime( time_t * sec = nullptr, int64_t * us = nullptr );
    string			API   GetTime_ISO8601( bool use_microseconds = true );
    time_t			API   DateString2Time(const string date, const string format, std::tm *t = 0, int64_t *us = 0);
    string		    TimeStampShort(struct std::tm *tout = 0, struct std::tm *tin = 0, int64_t *us = 0);
    static  void            SetExternalTimeSource(  std::function<double()>  funct ); 


#ifdef _WIN32
	static GTime API Now();
#endif

	int64_t GetBaseUsTime() { return(fTimeVal); }
  
    bool operator!=(GTime& o) { return(fTimeVal != o.fTimeVal); }
    bool operator==(GTime& o) { return(fTimeVal == o.fTimeVal); }
    bool operator>(GTime& o) { return(fTimeVal > o.fTimeVal); }
    bool operator>=(GTime& o) { return(fTimeVal >= o.fTimeVal); }
    bool operator<(GTime& o) { return(fTimeVal < o.fTimeVal); }
    bool operator<=(GTime& o) { return(fTimeVal <= o.fTimeVal); }
    
    GTimeSpan operator-(GTime& o) { return(GTimeSpan(fTimeVal - o.fTimeVal)); }

private:
    bool  ValidateV2(  std::function<bool(  const int ) > funct, const string &s, const vector<string> * const arr = nullptr );
    int64_t fTimeVal;
    GTimeFormat  fTimeFormat;
    GTimeValidate fTimeValidate; 
    static std::function<double()>  fExternalTimeSource; ///! External time source for epoch time
};





