// -*- mode: c++ -*-

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

#include "TestGText.h"
#include <utilities/GText.h>


TestGText::TestGText()
{
}


TestGText::~TestGText()
{

}



TEST_F(TestGText, SimpleGTextFormatting)
{
   EXPECT_EQ("Hello Dolly", GText("Hello Dolly").str() );
   EXPECT_EQ("a simple string with parameters: a = 1, b = 2", GText("a simple string with parameters: a = %d, b = %d", 1, 2).str() );
   EXPECT_STREQ("a simple C string with parameters: a = 1, b = 2", GText("a simple C string with parameters: a = %d, b = %d", 1, 2).c_str());
   EXPECT_STREQ("a simple C string with float parameters: a = 1.123, b = 2.34568", GText("a simple C string with float parameters: a = %0.3f, b = %0.5f", 1.123456, 2.3456789).c_str());
   // GText("Hello Dolly");
}



//#ifdef _WIN32
//NSR-202 NSR-204
TEST_F(TestGText, SprinfEmulator)
{
#define N 64
    char msg[N];
    GText(msg, N, "Hello Dolly");
    EXPECT_STREQ(msg, "Hello Dolly");

    ///The message written to the  msg buffer will be cut of after 63  characters, but the call to Txt.c_str() should still yield the full string
    EXPECT_STREQ ("Now we check what happens if we try to create a string more than 64 characters long", GText(msg, N, "Now we check what happens if we try to create a string more than %d characters long",  (int)N).c_str());
    EXPECT_STREQ(msg, "Now we check what happens if we try to create a string more th");
    EXPECT_STREQ(       "a simple C string with float parameters: a = 1.123, b = 2.34568", GText(msg, N, "a simple C string with float parameters: a = %0.3f, b = %0.5f", 1.123456, 2.3456789).c_str());
    EXPECT_STREQ(msg, "a simple C string with float parameters: a = 1.123, b = 2.3456");
    EXPECT_EQ("a simple C string with float parameters: a = 4.567, b = 7.89101", GText(msg, N, "a simple C string with float parameters: a = %0.3f, b = %0.5f", 4.567, 7.89101112).str());
    EXPECT_EQ(string(msg), "a simple C string with float parameters: a = 4.567, b = 7.8910");
    
}
///#endif