// -*- mode: c++ -*-


/*****************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                       ****
******************************************************************************/

/*****************************************************************************
******************************************************************************
*** This file is part of logmaster.                                        ***
*** Copyright (C) Per Thomas Hille <pth@embc.no>  http:///www.embc.no      ***
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

#include "GText.h"
#include <cstdarg>

#define GTEXT_MAX_SIZE 4096


GText::GText(const char * fmt, ...) : fText("")
{
    va_list ap;
    va_start(ap, fmt);
    SetGText(fmt, ap);
    va_end(ap);
 }


GText::GText(char *message, const int n, const char * fmt, ...) : fText("")
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(message, n-1 , fmt, ap);
    static char msg[GTEXT_MAX_SIZE];
    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg) -1, fmt, ap);
    fText = string(msg);
    va_end(ap);
}


GText::GText(const string s ) : fText("")
{   
   fText = s;
}


void
GText::SetGText(const char *fmt, va_list ap)
{
    static char msg[GTEXT_MAX_SIZE];
    vsnprintf(msg, sizeof(msg) -1, fmt, ap);
    va_end(ap);
    fText = string(msg);
}



GText & 
GText::operator  = (const char *rhs)
{
    this->SetGText(rhs);
    return *this;
}


void 
GText::SetGText(const char *txt)
{
    fText =  string(txt);
}


string    
GText::str() const 
{ 
    return fText; 

}


int64_t  
GText::size() const
{
    return fText.size();
}


const char *  
GText::c_str() const 
{ 
    return fText.c_str(); 
}


const char *
GText::what() const
{ 
    return fText.c_str(); 
}

