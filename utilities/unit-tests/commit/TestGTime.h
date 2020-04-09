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


#include <vector>
using std::vector;

#include <testlib/TestBase.h>

/** @brief unit tests for the GTime class */
class TestGTime : public TestBase
{
public:
    TestGTime();
    virtual ~TestGTime();
protected:
    void  Validate(bool(*funct)(const int val), bool(*funct2)(const string), const int min, const int max, const vector<string> valid, const 
        vector<string> invalid);
};



