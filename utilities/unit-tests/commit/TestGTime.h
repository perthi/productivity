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
#include <utilities/GTime.h>
#include <utilities/GTimeValidate.h>
#include <functional>

/** @brief unit tests for the GTime class */
class TestGTime : public TestBase
{
public:
    TestGTime();
    virtual ~TestGTime() {};
protected:

    GTime  fTime;
    GTimeValidate  fTimeValidate;
};

//  bool  ValidateV2(  std::function<bool(  const int ) > funct, const string &s, const vector<string> * const arr = nullptr );

