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

#include <string>
using std::string;
#include <typeinfo>
#include "GDefinitions.h"

#define GLOCATION  GLocation(__FILE__, __LINE__, __FUNCTION__)
#define SETPOS() { location = GLocation(__FILE__, __LINE__, __func__); }


/** @brief Helper class to stor information about source code location */
class GLocation
{
public:
    inline GLocation() {};
    inline GLocation(const string fname, const int lineno, const string funct ) : fFileName(fname), fLineNo(lineno), fFunctName(funct) {}
    string fFileName = "";
    int   fLineNo = -1;
    string fFunctName = "";

   inline const char * c_str() const
   {
       static string str_local;
       str_local = str();
       return str_local.c_str();
   }
    
    inline string str() const
    {
        static char loc[4096];
        SPRINTF(loc, 4096, "%s[line%d]: %s", fFileName.c_str() , fLineNo, fFunctName.c_str() );
        return  string(loc);
    }

};

