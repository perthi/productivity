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

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;

#include <string>
#include <sstream>
using std::string;

#include <type_traits>


#ifdef _WIN64f

#define __CALL__  _fastcall 
#else
#ifdef _WIN32
#define __CALL__  _stdcall 
#endif
#endif

#ifdef _WIN32
#define API __declspec(dllexport)
#else
#define API
#endif


#ifdef _WIN32
#define SPRINTF sprintf_s
#else
#define SPRINTF snprintf
#endif


#ifdef _WIN32
#define SPRINTF_S(buffer, input) sprintf_s(buffer, sizeof(buffer) -1, "%s", input.c_str() )
#else
#define SPRINTF_S(buffer, input) snprintf(buffer, sizeof(buffer) -1, "%s", input.c_str() ) 
#endif


#if defined(_WIN32) && (_MSC_VER) && _MSC_VER < 1500 || defined(_WIN64) && (_MSC_VER) && _MSC_VER < 1500
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

#ifdef _WIN32
#define strcasecmp _stricmp	
#endif


#define EXCECUTION_ERROR -33
#define ARG_MAX 4096

#define COUT  cout << __FILE__ << ":" <<__LINE__ <<":" << __FUNCTION__ <<":"

#if defined(__unix__)
#define CERR   cerr << "\033[7;1;34m" <<__FILE__ << ":" <<__LINE__ <<":" << __FUNCTION__ <<":" 
#define ENDL   "\033[0m" << endl;

#else
#define CERR  cerr << __FILE__ << ":" <<__LINE__ <<":" << __FUNCTION__ <<":"
#define ENDL endl
#endif

#define SHOW(a) COUT << #a << ": " << (a) << std::endl

#if _WIN64
typedef   wchar_t DBCHAR;
#else
typedef  unsigned char    DBCHAR;
#endif


#define  DAY_MIN 1
#define  DAY_MAX 7
#define  DATE_MIN 1
#define  DATE_MAX 31
#define  MONTH_MIN 1
#define  MONTH_MAX 12
#define  YEAR_MIN 2018
#define  YEAR_MAX 9999
#define  HOUR_MIN 0
#define  HOUR_MAX 24
#define  MINUTE_MIN 0
#define  MINUTE_MAX 60
#define  SECONDS_MIN 0
#define  SECONDS_MAX 60

#define ARG_OK true
#define ARG_ERROR false



#define CHECK_ARGS(type, cmd, parameter, lower, upper, status, msg)    \
    status = ARG_OK;                                                    \
    try                                                                 \
    {                                                                   \
        msg = g_utilities()->CheckLimits<type>((type)parameter.size(), lower, upper, #parameter, __FILE__, __LINE__, __FUNCTION__, &status); \
    }                                                                   \
    catch (...)                                                         \
    {                                                                   \
        if (status == ARG_ERROR)                                        \
        {    \
                if (lower == upper)                                         \
            {                                                           \
                RANGE_EXCEPTION("%s:\t%s invalid number of parameters(%d): %s takes exactly %d parameter", cmd.c_str(),msg.c_str(),  parameter.size(),   cmd.c_str(), upper); \
            }                                                           \
            else                                                        \
            {   \
                RANGE_EXCEPTION("%s:\t%s invalid number of parameters(%d): %s takes between %d and %d parameters", cmd.c_str(), msg.c_str(),  parameter.size(),  cmd.c_str(), lower, upper); \
            }                                                           \
                                                                        \
        } \
    }
 // Temp fix for ESCORE-1327, argc has been hardcoded to 1
#ifdef HAS_LOGGING
#include <cmdline/GLogApplication.h>
#define MAIN_UNITTEST() \
int argc_ = 0; \
char** argv_ = nullptr; \
int  main(int argc, char** argv) \
{ \
 	argc_ = argc; \
    argv_ = argv; \
        new GLogApplication( 1, (const char **)argv); \
\
	/* The method is initializes the Google framework and must be called before RUN_ALL_TESTS */ \
	::testing::InitGoogleTest(&argc, argv); \
\
        /* RUN_ALL_TESTS automatically detects and runs all the tests defined using the TEST macro.*/\
/* It's must be called only once in the code because multiple calls lead to conflicts and, */ \
/*therefore, are not supported. */ \
	return  RUN_ALL_TESTS(); \
} 

#else
#define MAIN_UNITTEST() \
int  main(int argc, char** argv) \
{ \
\
	/* The method is initializes the Google framework and must be called before RUN_ALL_TESTS */ \
	::testing::InitGoogleTest(&argc, argv); \
\
        /* RUN_ALL_TESTS automatically detects and runs all the tests defined using the TEST macro.*/\
/* It's must be called only once in the code because multiple calls lead to conflicts and, */ \
/*therefore, are not supported. */ \
	return  RUN_ALL_TESTS(); \
}

#endif


