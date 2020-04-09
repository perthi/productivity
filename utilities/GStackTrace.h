// -*- mode: c++ -*-

/****************************************************************************
*** Copyright(C) 2015  Per Thomas Hille, pth@embc.no                     ***
*** This file is part of logmaster. logmaster is free software : you can ***
*** redistribute it and / or modify it under the terms of the Lesser GNU ***
*** General Public License (LGPL) V3 or later. See .cpp file for details ***
*****************************************************************************/


#pragma once

#include <string>
#include <utilities/GDefinitions.h>

using std::string;

class GStackTrace// : public Printable
{
 public:
    GStackTrace();
    ~GStackTrace();
    static API string str();
    static API const char * c_str();
 private:
   
};



