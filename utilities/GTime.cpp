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
#include <thread>

#include <memory>
using namespace std::chrono;


GTime::GTime() :   fTimeVal(), fTimeFormat(), fTimeValidate()
{

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
            if ( fTimeValidate.IsGregorianLeapYear(i))
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

        if ( fTimeValidate.IsGregorianLeapYear(year) && month > 2)
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
 *  @return The number of sconds between Jauary 1st 00:00 1970  and the date represented by the "date" input parameter   */
time_t
GTime::DateString2Time(const string date, const string format, std::tm *t, int64_t *us )
{
    std::istringstream ss(date);
    std::tm s_t_l;
    ss >> std::get_time(&s_t_l, format.c_str());
    vector<string> tokens = GTokenizer().Tokenize(date, ".");
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
    char t[MAX_TIMESTAMP_SIZE];

    if (use_microseconds == true)
    {
#if defined(ARM) || defined(_WIN32)
        SPRINTF(t, MAX_TIMESTAMP_SIZE, "%s.%6lld", d.c_str(), us);
#else
		SPRINTF(t, MAX_TIMESTAMP_SIZE, "%s.%6ld", d.c_str(), us);
#endif
    }
    else
    {
        SPRINTF(t, MAX_TIMESTAMP_SIZE, "%s", d.c_str());
    }

    string utc = string(t) + TimeStamp("%z");
    SPRINTF_S(t,  utc );

    return string(t);
}



double   
GTime::GetEpochTime( time_t * sec, int64_t * us )
{
    system_clock::time_point tp = system_clock::now();
    system_clock::duration dtn = tp.time_since_epoch();

    double epoch_time = 0;
#ifdef _WIN32
   epoch_time = ((double)dtn.count())/10000000;

#else
   epoch_time = ((double)dtn.count())/1000000000;
#endif
    if( sec != nullptr )
    {
        *sec = (time_t)epoch_time;
    }
    if(  us != nullptr )
    {
        *us = dtn.count()  %  *sec;
    }
   return  epoch_time;
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
    string offender = "";
    char tmp[MAX_TIMESTAMP_SIZE] = {0};
    char f[MAX_TIMESTAMP_SIZE] = {0};

    if(  ( format != nullptr ) &&  ( fTimeValidate.IsValidFormat(format, offender ) == false ) )
    {

        std::stringstream buffer;
        
        buffer << "Illegal format specifier " << string(format) << "\"\n" +  fTimeFormat.str() << ", the offending specifier is \"" << offender << "\" which is unknown"
               << "\n"
               << "error in function " << __FUNCTION__ << "at:" << __FILE__ << ":" << __LINE__ << "\n";
        
        throw(std::runtime_error(buffer.str().c_str()));
    }
    else

    {
        struct std::tm timeinfo;
        std::time_t now;
        
        GetEpochTime(&now, us);
      //  GetRawTime(&now, us);

        if (tin ==  nullptr )
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

        if (tout !=  nullptr )
        {
            *tout = timeinfo;
        }

        if (format == nullptr )
        {
            snprintf(f, MAX_TIMESTAMP_SIZE, "%s", "[%a %d %B-%Y %H:%M:%S]");
        }
        else
        {
            snprintf(f, MAX_TIMESTAMP_SIZE, "%s", format);
        }

        std::strftime(tmp, MAX_TIMESTAMP_SIZE - 1, f, &timeinfo);
        return string(tmp);
    }
}


