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

#include <stdint.h>


/** @brief Convert a time duration in micro seconds  to seconds, minutes, hours etc..*/
class GTimeSpan
{
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




