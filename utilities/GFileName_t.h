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


/* @brief Helper class to distinguish between a regular string and a filename. This is needed by the commandline handling
which will interpret a regular string as command line*/
class GFileName_t
{
public:
    GFileName_t(const string fname) : fFileName(fname){};
    virtual ~GFileName_t() {};
    void Set(string fname) { fFileName = fname; }
    string Get() const {  return fFileName ; };
    string str() const { return fFileName; };
    const char * c_str() { return fFileName.c_str(); };

private:
    GFileName_t();

    string fFileName;
};

