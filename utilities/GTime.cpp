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

#include "GTime.h"


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
using namespace std::chrono;

namespace {
vector<string> s_validMonths = {
    "January", "February", "March",     "April",   "May",      "June",
    "July",    "August",   "September", "October", "November", "December"};
vector<string> s_validDays = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
std::tm s_t_l;
static map<string, string> s_format = {
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
}


GTime * g_time()
{
    static GTime *instance = new GTime();
    return instance;
}



GTime::GTime(int year, int month, int dayInMonth, int hour24, int minute, int second, int us) : fTimeVal(0)
{
	SetTime(year, month, dayInMonth, hour24, minute, second, us);
}


#ifdef _WIN32
GTime GTime::Now()
{
	FILETIME fileTime;
	SYSTEMTIME systemTime;
	TIME_ZONE_INFORMATION timeZoneInformation;
	SYSTEMTIME localTime;

	GetTimeZoneInformation(&timeZoneInformation);
	GetSystemTimePreciseAsFileTime(&fileTime);
	FileTimeToSystemTime(&fileTime, &systemTime);
	SystemTimeToTzSpecificLocalTime(&timeZoneInformation,
		&systemTime,
		&localTime);

	ULARGE_INTEGER ul;
	ul.HighPart = fileTime.dwHighDateTime;
	ul.LowPart = fileTime.dwLowDateTime;
	int us = (int)((ul.QuadPart / 10) % 1000000);
	return ( GTime(localTime.wYear, localTime.wMonth, localTime.wDay, localTime.wHour, localTime.wMinute, localTime.wSecond, us));
}
#endif


void
GTime::SetTime(int year, int month, int dayInMonth, int hour24, int minute, int second, int us)
{
	std::tm tm;
	::memset(&tm, 0, sizeof(tm));
	tm.tm_year = year - 1900;
	tm.tm_mon = month;
	tm.tm_mday = dayInMonth;
	tm.tm_hour = hour24;
	tm.tm_min = minute;
	tm.tm_sec = second;

	int64_t timeSeconds = (int64_t)mktime(&tm);
	fTimeVal = timeSeconds * 1000000 + us;
}





// #define YEAR_MIN 1999

/*
 * @class GTime class for 
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




vector<string> GTime::GetFormatSpecifiers()
{
    vector<string> tmp;
    std::map<string, string>  format = FormatChars();

    for (std::map<string, string>::iterator iter = format.begin(); iter != format.end(); ++iter)
    {
        tmp.push_back(iter->first);
    }

    return tmp;
}



/*
  NSR - 242
  * Checks if a Year is a leap year.
  * @param[in] iYear
  * @return true if Leap Year othervise false
  */
bool GTime::IsGregorianLeapYear(int iYear)
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



//CRAP PTH, refactor
bool
GTime::IsValidDateString(const string t)
{
    if (t.size() == 0)
    {
        return false;
    }

        char day[64] = {0};
    int  date;
        char month[64] = {0};
    int  year;
    int  hour;
    int  minute;
    int  second;


#ifdef _WIN32
    int  ret = sscanf_s(t.c_str(),  "[%s %d %s %d:%d:%d]", day, 64, &date, month, 64, &hour, &minute, &second );
    if (ret != 6)
    {
        std::stringstream buffer;
        buffer << "could not scan string " << t << " scanf returned " << ret << ", expected 6 paramters " << "\n";
#ifdef _WIN32
        throw(std::exception(buffer.str().c_str()));
#else
        throw(std::runtime_error(buffer.str().c_str()));
#endif
    }
#else
    sscanf(t.c_str(),  "[%s %d %s %d:%d:%d]", day, &date, month, &hour, &minute, &second );
#endif
    vector<string> ytokens = g_tokenizer()->Tokenize(month, "-");

    if (ytokens.size() == 2)
    {
        year = g_numbers()->ToInteger<int>(ytokens[1]);
        snprintf(month, 64, "%s", ytokens[0].c_str());
    }
    else
    {
        CERR << "ERROR; invalid GTime  string;\t " << t << ", GTime.size() = " << t.size() <<  ENDL;
        return false;
    }

  
    bool isValidDay = false;
    bool isValidDate = false;
    bool isValidMonth = false;
    bool isValidYear = false;
    bool isValidHour = false;
    bool isValidMinute = false;
    bool isValidSecond = false;

    string sday = string(day);
    for (uint16_t i = 0; i < s_validDays.size(); i++)
    {
        if (sday == s_validDays[i])
        {
            isValidDay = true;
            break;
        }
    }

    if (date > 0 && date <= 31)
    {
        isValidDate = true;
    }


    /// NB will fail in year 9999 !!!   
    if (year >= YEAR_MIN && year < 9999)
        //  if (year > 1999 && year < 9999)    
    {
        isValidYear = true;
    }
    else
    {
        CERR << "Invalid year " << year << ", year must be between " << YEAR_MIN << " and " << YEAR_MAX << ENDL;
    }

    string smonth = string(month);
    for (uint16_t i = 0; i < s_validMonths.size(); i++)
    {
        if (smonth == s_validMonths[i])
        {
            
            isValidMonth = true;
            break;
        }
    }

    if (hour < 24 && hour >= 0) { isValidHour = true; }
    if (minute < 60 && minute >= 0) { isValidMinute = true; };
    if (second < 60 && second >= 0) { isValidSecond = true; };


    if (isValidDay && isValidDate && isValidMonth && isValidYear && isValidHour  && isValidMinute  && isValidSecond)
    {
        return true;
    }
    else
    {
      //  COUT << ":ERROR; invalid GTime  string;\t " << t << endl;
        return false;
    }

}


bool GTime::IsValidDay(string day)
{
    vector<string> valid_days = { "monday",  "tuesday",  "wednesday",  "thursday", "friday", "saturday", "sunday" };
    return Validate( IsValidDay, day, valid_days);
}


bool  GTime::IsValidDay(const int day)
{
    return g_utilities()->IsInRange(day, DAY_MIN, DAY_MAX);
}


bool GTime::IsValidDate(const string date)
{
    return Validate(IsValidDate, date);
}


bool GTime::IsValidDate(const int date)
{
    return g_utilities()->IsInRange(date, DATE_MIN, DATE_MAX);
}



bool GTime::IsValidMonth(const string month)
{
    vector<string> valid_months{ "january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december" };
    return Validate(IsValidMonth, month, valid_months);
    return false;
}


bool  GTime::IsValidMonth(const int month)
{
    return g_utilities()->IsInRange(month, MONTH_MIN, MONTH_MAX);
}


bool GTime::IsValidYear(const string year)
{
    return Validate(IsValidYear, year);
}


bool  GTime::IsValidYear(const int year)
{
    return g_utilities()->IsInRange(year, YEAR_MIN, YEAR_MAX);
    
}


bool GTime::IsValidHour(const string hour)
{
    return Validate(IsValidHour, hour);
}


bool  GTime::IsValidHour(const int hour)
{
    return g_utilities()->IsInRange(hour, HOUR_MIN, HOUR_MAX);
}


bool  GTime::IsValidMinute(const string m)
{
    return Validate(IsValidMinute, m);
}


bool  GTime::IsValidMinute(const int m)
{
    return g_utilities()->IsInRange(m, MINUTE_MIN, MINUTE_MAX);
}


bool GTime::IsValidSecond(const string s)
{
    return Validate(IsValidSecond, s); 
}



bool  GTime::IsValidSecond(const int s)
{
    return g_utilities()->IsInRange(s, SECONDS_MIN,  SECONDS_MAX);
}



bool
GTime::Validate(bool(*funct)(const int), const string s, vector<string> arr)
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


/** Todays GTime and date on a format understood by the Access databas. In Access the dat is represented as a
 *  single number (double) as the number of days since 1 Jan 1900 on decismnal form (tha is, for instance 0.5 days = 12 hours )
 *  The dat is taken from the operatinmg system 
 *  @return The number of days since 1.January 1900 00:00  on decimal form */
long double GTime::AccessDate()
{
    string date = TimeStamp("%Y:%m:%u:%H:%M:%S");
    return AccessDate(date);
}


/** Date and GTime converter form the input GArguments to a format understood by the Access databas. In Access the data is represented as a
 *  single number (double) as the number of days since 1 Jan 1900 on decimal form (that is, for instance 0.5 days = 12 hours )
 *  @param date on string format which assumes the format yyyy:mm:dd:hh:mm:ss  The separator is insignificant
 *  @return The date on Access format */
long double  GTime::AccessDate(const string date)
{
    int mdays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    long double DAYS_1900_TO_2000 = 36526.0;
    long double nDays = DAYS_1900_TO_2000;

    vector<string> numbers = g_regexp()->ScanNumber(date, "1,12");
    
    if (numbers.size() != 6)
    {
        //	cerr << __FILE__ << __LINE__ << ": " << __FUNCTION__ << "Incorrect Date format" << date;
        //EXCEPTION("Incorrect Date format %s (the count of numbers fields is %d, expected 6)", date.c_str(), numbers.size());
        std::stringstream buffer;
        buffer << "Incorerect date format " << date << " the count of number fields is " << numbers.size() << " expected 6" << "\n";
#ifdef _WIN32
        throw(std::exception(buffer.str().c_str()));
#else
        throw(std::runtime_error(buffer.str().c_str()));
#endif

    }
    else
    {
        int year =  g_numbers()->ToInteger<int>(numbers[0]);
        int month = g_numbers()->ToInteger<int>(numbers[1]);
        int day =   g_numbers()->ToInteger<int>(numbers[2]);
        int hour =  g_numbers()->ToInteger<int>(numbers[3]);
        int min =   g_numbers()->ToInteger<int>(numbers[4]);
        int sec =   g_numbers()->ToInteger<int>(numbers[5]);

        for (int i = 2000; i < year; i++)
        {
            if (IsGregorianLeapYear(i))
            {
                nDays += 366;
            }
            else
            {
                nDays += 365;
            }
        }

        for (int i = 0; i < month - 1; i++)
        {
            nDays += mdays[i];
        }

        if (IsGregorianLeapYear(year) && month > 2)
        {
            nDays += 1;
        }

        nDays += (long double)day - (long double)1;
		int64_t minutes_per_day = 24 * 60;
		int64_t seconds_per_day = minutes_per_day*60;
		long double rem = (long double)hour / 24 + (long double)min / (minutes_per_day) + (long double)sec / (seconds_per_day );
        nDays += rem;
    }
    return nDays;
}



/*  Converts a date on string format to the number of seconds since January 1st 1970. Optionally gnerates std:tm struct an calculates the number of microseconds 
 *   by optional in/out parameters if the date string is on the ISO8601 format.    
 *  @param[in]  date  The date that should be converted
 *  @param[in]  format  The format of the date string. NB the format must match the date. Othervise aal fileds in the "tm" structy is set to ZERO.   
 For instance 
 date =  "1988-June-23 10:09:49";
 must be parsed with 
 format = "%Y-%b-%d %H:%M:%S",
  
 *  @return The number of sconds between Jauary 1st 00:00 1970  and the date represented by the "date" input parameter  
 */
time_t
GTime::DateString2Time(const string date, const string format, std::tm *t, int64_t *us )
{
    std::istringstream ss(date);
    ss >> std::get_time(&s_t_l, format.c_str());
    vector<string> tokens = g_tokenizer()->Tokenize(date, ".");
    size_t n = tokens.size();
    
    if (us != 0)
    {
        if (n == 1)
        {
            *us = 0;
        }
        else
        {
            string snum = tokens.at(n - 1);
            if (g_numbers()->IsNumber(snum))
            {
                snum = "." + snum;
                double d = g_numbers()->ToFloat(snum);
                *us = (int64_t)(d * 1000000);
            }
            else
            {
                *us = 0;
            }
        }
    }
  
    if (t != 0)
    {
        *t = s_t_l;
    }

    return   mktime(&s_t_l);
}



string
GTime::GetTime_ISO8601(bool use_microseconds)
{
    int64_t us;
    string d = TimeStamp("%FT%T", &us);
    
    return d;
    
    static char t[512]; /// @todo remove magic number

    if (use_microseconds == true)
    {
#ifdef ARM
        SPRINTF(t, 512, "%s.%6lld", d.c_str(), us);
#else
#ifdef _WIN32
        SPRINTF(t, 512, "%s.%6lld", d.c_str(), us);
#else
		SPRINTF(t, 512, "%s.%6ld", d.c_str(), us);
#endif
#endif
    }
    else
    {
        SPRINTF(t, 512, "%s", d.c_str());
    }

    string utc = TimeStamp("%z");

    SPRINTF_S(t, utc );

    return string(t);
}



double   
GTime::GetEpochTime()
{
    system_clock::time_point tp = system_clock::now();
    system_clock::duration dtn = tp.time_since_epoch();
    //	return dtn.count() * system_clock::period::num / system_clock::period::den;
#ifdef _WIN32
    return ((double)dtn.count())/10000000;
#else
    return ((double)dtn.count())/1000000000;
#endif
}



/* @param[in|out] seconds Number of seconds since 1.january 1970
 *  @param[in|out] time since epoch in microseconds, the value is arbritrary and should be used for comparison only */
void  
GTime::GetRawTime(time_t * sec, int64_t * us)
{
    if (sec != 0)
    {
        *sec = std::time(0);
    }
    if (us != nullptr)
    {
        auto lus = duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch());
        *us = lus.count() % 1000000;
        //*us = lus.count();

    }
}



string
GTime::TimeStampShort(struct std::tm *  tout, struct std::tm * tin, int64_t * us )
{
   return TimeStamp(tout, "%H:%M:%S" , tin, us);
}


string
GTime::TimeStamp(const char * format, int64_t *us)
{
    return TimeStamp(0, format, 0, us);
}


string
GTime::TimeStamp(struct std::tm *tout, const char *format, struct std::tm *tin, int64_t *us)
{
    string offender;
    if (IsValidFormat(format, offender) == false)
    {

        std::stringstream buffer;
        buffer << "Illegal format specifier " << string(format) << "\"\n" + str() << ", the offending specifier is \"" << offender << "\" which is unknown"
               << "\n"
               << "error in function " << __FUNCTION__ << "at:" << __FILE__ << ":" << __LINE__ << "\n";
        ///@todo use GLOCATION
        throw(std::runtime_error(buffer.str().c_str()));
    }
    else

    {
        char tmp[512] = {0};
        char f[512] = {0};
        //struct std::tm *timeinfo = 0;
        struct std::tm timeinfo;
        std::time_t now;
        GetRawTime(&now, us);

        if (tin == 0)
        {
            #ifndef _WIN32
                timeinfo = *localtime(&now);
            #else
                localtime_s(&timeinfo, &now);
            #endif
        }
        else
        {
            timeinfo = *tin;
        }

        if (tout != 0)
        {
            *tout = timeinfo;
        }

        if (format == 0)
        {
            snprintf(f, 512, "%s", "[%a %d %B-%Y %H:%M:%S]");
        }
        else
        {
            snprintf(f, 512, "%s", format);
        }

        std::strftime(tmp, 512 - 1, f, &timeinfo);
        return string(tmp);
    }
}


map<string, string> &GTime::FormatChars() {
    return s_format;
}

bool GTime::IsValidFormat(const char *c, string &offender)
{
    //  Initialize();
    bool has_valid_token = false;
    bool has_invalid_token = false;

    if (c == 0)
    {
        return true;
    }
    else
    {
        vector<string> tokens = g_tokenizer()->Tokenize(c, "%");

        for (unsigned int i = 0; i < tokens.size(); i++)
        {
            bool isvalid = false;
            string tmp = "";
            if (tokens[i].size() >= 1)
            {
                tmp = "%" + tokens[i].substr(0, 1);
            }
            for (auto const &item : s_format)
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
        }
    }

    if (has_invalid_token == true)
    {
        return false;
    }
    if (has_valid_token == true)
    {
        return true;
    }
    return false;
}



string
GTime::str()
{
    string infostring = "******  Valid Format Specifiers  BEGIN ******\n";
    for (std::map<string, string>::iterator iter = s_format.begin(); iter != s_format.end(); ++iter)
    {
        infostring += iter->first + ":\t" + string(iter->second) + "\n";

    }

    infostring += "\n ***** Valid format specifiers END ******\n";
    return infostring;
}
