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

class GTime;

GTime * g_time();

/** @brief Convert a time duration in micro seconds  to seconds, minutes, hours etc..*/
class GTimeSpan
{
    friend GTime * g_time();

public:
    GTimeSpan(const int64_t usTimeSpan) : fTimespan(usTimeSpan)  {  }
    int64_t GetMicroSeconds() { return(fTimespan); }
    int64_t GetMilliSeconds() { return(fTimespan / 1000); }
    int64_t GetSeconds() { return(fTimespan / 1000000); }
    int64_t GetMinutes() { return(fTimespan / 60000000); }
    int64_t GetHours() { return(fTimespan / 3600000000); }
    int64_t GetDays() { return(fTimespan / 86400000000); }
private:
    int64_t fTimespan;
};


/** @brief various utility for acess to system time which is can be availabel on a user specifed format
 *  @details Also contains function for leap-year checks and validtiy check for days, months etc.. */
class GTime 
{
public:
    GTime() :   fTimeVal(0) {};
	API GTime(int year, int month, int dayInMonth, int hour24 = 0, int minute = 0, int second = 0, int us = 0);
    ~GTime() {};
    bool operator!=(GTime& o) { return(fTimeVal != o.fTimeVal); }
    bool operator==(GTime& o) { return(fTimeVal == o.fTimeVal); }
    bool operator>(GTime& o) { return(fTimeVal > o.fTimeVal); }
    bool operator>=(GTime& o) { return(fTimeVal >= o.fTimeVal); }
    bool operator<(GTime& o) { return(fTimeVal < o.fTimeVal); }
    bool operator<=(GTime& o) { return(fTimeVal <= o.fTimeVal); }
    GTimeSpan operator-(GTime& o) { return(GTimeSpan(fTimeVal - o.fTimeVal)); }
    void API SetTime(int year, int month, int dayInMonth, int hour24 = 0, int minute = 0, int second = 0, int us = 0);
    
    static long double		API   AccessDate();
    static long double		API   AccessDate(const string date);
    static bool				API   IsGregorianLeapYear(int iYear);
    static bool				API   IsValidDateString(const string GTime);
    static bool				API   IsValidDay(    string  day );
    static bool				API   IsValidDay(   const int  day);
    static bool				API   IsValidDate(  const string  date); 
    static bool				API   IsValidDate(  const int  date);
    static bool				API   IsValidMonth( const string  month);
    static bool				API   IsValidMonth( const int     month);
    static bool				API   IsValidYear(  const string  year);
    static bool				API   IsValidYear(  const int     year);
    static bool				API   IsValidHour(  const string  hour);
    static bool				API   IsValidHour(  const int     hour);
    static bool				API   IsValidMinute( const string  m);
    static bool				API   IsValidMinute( const int     h);
    static bool				API   IsValidSecond( const string  s);
    static bool				API   IsValidSecond( const int     s);
    static bool				API   IsValidFormat(const char  *c,  string & offender);
    vector<string> static	API   GetFormatSpecifiers();
    static  map<string, string> API & FormatChars();
    static string			API   str();
    static string			API   TimeStamp(struct std::tm *tout = 0, const char * format = 0, struct std::tm *tin = 0, int64_t *us = 0);
    static string			API   TimeStamp(const char * format, int64_t *us = 0 );
    static void				API   GetRawTime(time_t *seconds, int64_t  *us = nullptr);
    static double            API   GetEpochTime();
    static string			API   GetTime_ISO8601( bool use_microseconds = true );
    static time_t			API   DateString2Time(const string date, const string format, std::tm *t = 0, int64_t *us = 0);
    string		TimeStampShort(struct std::tm *tout = 0, struct std::tm *tin = 0, int64_t *us = 0);
    
#ifdef _WIN32
	static GTime API Now();
#endif

	int64_t GetBaseUsTime() { return(fTimeVal); }

private:


    static bool  Validate(bool( * funct)(const int), const string s, vector<string> arr = vector<string>() );
    int64_t fTimeVal;
    char fBuffer[512] = {0};
    char fFormat[512] = {0};
};










