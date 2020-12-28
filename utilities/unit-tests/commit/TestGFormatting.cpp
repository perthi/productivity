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

#include "TestGFormatting.h"

#ifdef HAS_LOGGING
#include <exception/GException.h>
#include <logging/LLogApi.h>
#include <logging/LPublisher.h>
using namespace LOGMASTER;
#endif

void 
TestGFormatting:: SetUpTestCase()
{
   #ifdef HAS_LOGING 
    SET_LOGTARGET("--target-stdout");
    SET_LOGLEVEL("--all-off --all-debug");
    LPublisher::Instance()->SetMode(ePUBLISH_MODE::SYNCHRONOUS );
    #endif
}




#ifdef HAS_LOGGING
TEST_F(TestGFormatting,  format1 )
{
    int t1 = 1;
    int t2 = 2;
    string s = "test";
    EXPECT_NO_THROW( G_ERROR("t1 = %d, t2= %d", t1, t2 ) );
    EXPECT_NO_THROW( G_ERROR("t1 = %d, s= %s, t2 = %d", t1,  s.c_str(), t2 ) ); /// arguments in correct order
    EXPECT_THROW( G_ERROR("t1 = %d, t2= %s", t1, t2),  GInvalidArgumentException  ); /// last argumen of wrong type
    EXPECT_THROW( G_ERROR("t1 = %d", t1, t2 ),GInvalidArgumentException  ); /// too many arguements
    EXPECT_THROW( G_ERROR("t1 = %d, s= %s, t2 = %d", t1, t2, s.c_str() ), GInvalidArgumentException  ); /// arguments in wrong order
    EXPECT_THROW( G_ERROR("t1 = %d, s= %s, t2 = %d", t1,  s.c_str() ),  GMissingArgumentException); /// Missing argument

}
#endif


#ifdef HAS_LOGGING
/*
TEST_F(TestGFormatting,  unnecessary_check )
{
    int t1 = 1;
    int t2 = 2;
    string s = "test";
    EXPECT_NO_THROW( G_DEBUG("t1 = %d, s= %s, t2 = %d", t1, t2, s.c_str() )); /// no exception expecteed, arguments in wrong order but loglevel is WARNING
}
*/
#endif


#ifdef HAS_LOGGING
TEST_F(TestGFormatting,  format2 )
{
    unsigned long long test = 123456;
    EXPECT_NO_THROW( G_ERROR("test = %d", test )) ;
    EXPECT_NO_THROW( G_ERROR("test = %ld", test )) ;
    EXPECT_NO_THROW( G_ERROR("test = %lld", test )) ;
    EXPECT_NO_THROW( G_ERROR("test = %u", test )) ;
    EXPECT_NO_THROW( G_ERROR("test = %lu", test )) ;
    EXPECT_NO_THROW( G_ERROR("test = %llu", test )) ;
}
#endif


#ifdef HAS_LOGGING
/*
TEST_F(TestGFormatting, ESCORE_1253 )
{
    int t1 = 1;
    int t2 = 2; 
    EXPECT_ANY_THROW( G_ERROR("t1 = %d, t2= %d" ) );
}
*/
#endif
