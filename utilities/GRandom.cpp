// -*- mode: c++ -*-

#include "GRandom.h"


/*****************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                       ****
******************************************************************************/

/*****************************************************************************
******************************************************************************
*** This file is part of logmaster.                                        ***
*** Copyright (C) 2018 Per Thomas Hille <pth@embc.no> http:///www.embc.no  ***
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


// #include "GRandom.h"
// #include <limits.h>
// #include "GDefinitions.h"

////#ifdef _WIN32
//string 
//GRandom::Name( const string prefix, const string postfix)
//{
//    static char name[4096];
//    unsigned short num = Uniform<unsigned short>(  0,  USHRT_MAX );
//    SPRINTF(name, 4096-1, "%s%05d%s", prefix.c_str(), num, postfix.c_str() );
//    //    sprintf(name, "%s%05d%s", prefix.c_str(), num, postfix.c_str() );
//    return string(name);   
//}
////#endif


/*
const std::default_random_engine 
g_random()->Seed()
{
    std::random_device r;
    std::default_random_engine e(r());
    return e;
}
*/

