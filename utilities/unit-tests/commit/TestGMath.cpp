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

#include "TestGMath.h"
#include <utilities/GMath.h>


TestGMath::TestGMath()
{

}


TestGMath::~TestGMath()
{

}


TEST_F(TestGMath, GodenCut)
{
    EXPECT_DOUBLE_EQ(1.6180339887498949, g_math()->GoldenLongEdge());
    EXPECT_DOUBLE_EQ(0.6180339887498949, g_math()->GoldenShortEdge());
}



TEST_F(TestGMath, MinMax)
{
    EXPECT_EQ(100, g_math()->Max(10, 100));
    EXPECT_EQ(-99, g_math()->Max( -99, -999));
    EXPECT_EQ(10, g_math()->Min( 10, 100));
    EXPECT_EQ(-999, g_math()->Min(-99, -999));

    EXPECT_EQ(INT64_MAX, g_math()->Max(INT64_MAX, INT64_MAX -1));
    EXPECT_EQ(INT64_MAX -1, g_math()->Min(INT64_MAX, INT64_MAX - 1));

    EXPECT_EQ(INT64_MIN, g_math()->Min(INT64_MIN, INT64_MIN + 1));
    EXPECT_EQ(INT64_MIN + 1, g_math()->Max(INT64_MIN, INT64_MIN + 1));

    EXPECT_EQ(INT64_MIN, g_math()->Min(INT64_MIN +1, INT64_MIN ));
    EXPECT_EQ(INT64_MIN + 1, g_math()->Max(INT64_MIN +1, INT64_MIN));

    EXPECT_EQ(INT64_MIN + 1, g_math()->Max(INT64_MIN + 1, INT64_MIN +1));
    EXPECT_EQ(INT64_MIN + 1, g_math()->Min(INT64_MIN + 1, INT64_MIN + 1));
}



TEST_F(TestGMath, Abs)
{
    EXPECT_EQ(99, g_math()->Abs(-99));
    EXPECT_EQ(2147483647, g_math()->Abs(-2147483647));
    EXPECT_EQ(9223372036854775807, g_math()->Abs(-9223372036854775807));
}


TEST_F(TestGMath, IsEqual_NSR1038)
{   
    /** its ok to get warnings here, disabling them */    
#pragma warning(suppress: 4305)
    EXPECT_TRUE(  g_math()->IsEqual<float>(  1.6180339887498949, 1.6180339887498949));
#pragma warning(suppress: 4305)
    EXPECT_TRUE(  g_math()->IsEqual<float>(  1.6180339887498949, 1.6180339887498900));

    EXPECT_TRUE(  g_math()->IsEqual<double>(  1.6180339887498949 , 1.6180339887498949  ));
    EXPECT_FALSE( g_math()->IsEqual<double>(  1.6180339887498949, 1.6180339887498900));
    EXPECT_TRUE(  g_math()->IsEqual<long double>(  1.6180339887498949, 1.6180339887498949));
    EXPECT_FALSE( g_math()->IsEqual<long double>( 1.6180339887498949, 1.6180339887498900));
#pragma warning(suppress: 4244)
    EXPECT_TRUE(  g_math()->IsEqual<int>(1.2, 1.3));
    EXPECT_FALSE(  g_math()->IsEqual<int>(1, 2));
    EXPECT_FALSE(  g_math()->IsEqual<int>(100, 200));
}