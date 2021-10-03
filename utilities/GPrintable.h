// -*- mode: c++ -*-


#ifndef GPRINTABLE_H
#define GPRINTABLE_H

/****************************************************************************
*** Copyright(C) 2015  Per Thomas Hille, pth@embc.no                     ***
*** This file is part of logmaster. logmaster is free software : you can ***
*** redistribute it and / or modify it under the terms of the Lesser GNU ***
*** General Public License (LGPL) V3 or later. See .cpp file for details ***
*****************************************************************************/


#include <string>
using std::string;
using std::ostream;
#include <sstream>


/** @brief    Base class for "printable" classes (to stdout) 
 *  @details  All classes that dervies from this class
 * is printable with cout using the << operator. The user must 
 * implement the str() functions. The return value of this functions
 * is what will be printed when using cout etc.. on the obaject. */
class  GPrintable
{
public:
    inline   GPrintable() {};

    inline   virtual ~GPrintable( ) {};
   // virtual  string str() const = 0;
   // virtual string str() const { return "not implemented"; };
    virtual string str() const = 0;

};



inline
ostream& operator<<(ostream& os, const GPrintable &o)
{
    os << o.str();
    return os;
}

#endif
