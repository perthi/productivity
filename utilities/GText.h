// -*- mode: c++ -*-

#pragma once

/****************************************************************************
*** Copyright(C) 2015  Per Thomas Hille, pth@embc.no                     ***
*** This file is part of logmaster. logmaster is free software : you can ***
*** redistribute it and / or modify it under the terms of the Lesser GNU ***
*** General Public License (LGPL) V3 or later. See .cpp file for details ***
*****************************************************************************/

#pragma once

#include <string>
using std::string;
#include "GPrintable.h"
#include "GDefinitions.h"

#include <stdint.h>


/** @brief Generate a text object using printf formatting */
class GText : public GPrintable
{
public:
   API GText (const char * fmt, ... );
   API GText (char *message, const int n, const char * fmt, ...);
   API GText (const string s );
   string             API    str() const  override;
   int64_t            API    size() const;
   const              API    char *    what() const;
   const char         API  * c_str() const;
   void               API    SetGText(const char *txt);
  
   GText &  operator  = (const char *rhs);
   void                SetGText(const char *fmt, va_list ap);

private:
  
    GText() : fText("") {};
    string fText;
};


inline string operator  + (const GText &lhs, const string &rhs);
inline string operator  + (const string &lhs, const GText &rhs);
inline bool operator  == (const GText &lhs, const string &rhs);
inline bool operator  == (const string &lhs, const GText &rhs);
inline bool operator  != (const GText &lhs, const string &rhs);
inline bool operator  != (const string &lhs, const GText &rhs);



inline string operator  + (const GText &lhs, const string &rhs)
{
    return  lhs.str() + rhs;
}


inline string operator  + (const string &lhs, const GText &rhs)
{
    return  lhs + rhs.str();
}


inline bool operator  == (const GText &lhs, const string &rhs)
{
    if (lhs.str() == rhs)
    {
        return true;
    }
    else
    {
        return false;
    }
}


inline bool operator  == (const string &lhs, const GText &rhs)
{
    if (rhs.str() == lhs)
    {
        return true;
    }
    else
    {
        return false;
    }
}


inline bool operator  != (const GText &lhs, const string &rhs)
{
    if (lhs.str() == rhs)
    {
        return  false;
    }
    else
    {
        return true;
    }
}


inline bool operator  != (const string &lhs, const GText &rhs)
{
    if (rhs.str() == lhs)
    {
        return false;
    }
    else
    {
        return true;
    }
}
