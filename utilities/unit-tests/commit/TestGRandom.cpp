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

#include "TestGRandom.h"
#include <utilities/GRandom.h>
#include <utilities/GText.h>
#include <utilities/GLocation.h>
#include <utilities/GCommon.h>

TestGRandom::TestGRandom()
{
}


TestGRandom::~TestGRandom()
{
}


TEST_F(TestGRandom, numberGenerationNSR331)
{
    #ifdef HAS_LOGGING
    //eMSGTARGET previous = l->GetLogTarget();
    l->Push( );
    SET_LOGTARGET("0000 --target-file");
    #endif

    int num1 = g_random()->Uniform<int>(50,2000);
    EXPECT_GE(num1, 50);
    EXPECT_LE(num1, 2000);
    
    int num2 = g_random()->Uniform<int>(200, 500);
    EXPECT_GE(num2, 200);
    EXPECT_LE(num2, 500);
    
    int num3 = g_random()->Uniform<int>(-1001, -100);
    EXPECT_GE(num3, -1001);
    EXPECT_LE(num3, -100);

    double d1 = g_random()->Uniform<double>(-50, 50);
    EXPECT_GE(d1, -50);
    EXPECT_LE(d1, 50);

    double d2 = g_random()->Uniform<double>(10000, 20000);
    EXPECT_GE(d2, 10000);
    EXPECT_LE(d2, 20000);

    float f1 = g_random()->Uniform<float>(-200, -10);
    EXPECT_GE(f1, -200);
    EXPECT_LE(f1, -10);

    float f2 = g_random()->Uniform<float>(1, 2);
    EXPECT_GE(f2, 1);
    EXPECT_LE(f2, 2);
    
    /*We generate a GRandom gaussian number with mean and sigma, we expect the value
    * To be between mean +/- 6 sigma. This means that if the code works, the test will still fail 0.00034%
    * of the time, but thats acceptable */
    double mean = 100;
    double sigma = 10;
    double g1 = g_random()->Gauss<double>(mean, sigma);
    EXPECT_GE(g1, mean - 6*sigma);
    EXPECT_LE(g1, mean + 6*sigma);

    mean = -200;
    sigma = 4;
    double g2 = g_random()->Gauss<double>(mean, sigma);
    EXPECT_GE(g2,  mean - 6*sigma);
    EXPECT_LE(g2,  mean + 6*sigma);

    /*Check that we get an exception if sigma is negative*/
    #ifdef HAS_LOGGING
    EXPECT_ANY_THROW(  g_random()->Gauss<double>(1, -1)  );
    #else
    EXPECT_ANY_THROW(  g_random()->Gauss<double>(1, -1) ); 
    #endif

    int n = 10;
    double p = 0.5;

    for (int i = 0; i < 10; i++)
    {
        double tmp = g_random()->Binominal<int>(n, p);
        EXPECT_LE(tmp, 10);
        EXPECT_GE(tmp, 0);
    }

    double n2 = 20;
    float n3 = 30;
    EXPECT_ANY_THROW( g_random()->Binominal<double>( n2, p));
    EXPECT_ANY_THROW( g_random()->Binominal<float>(  n3, p));

    #ifdef HAS_LOGGING
    SET_LOGTARGET("0000 --target-file");
    l->Pop( );
    #endif

}



TEST_F(TestGRandom, filename)
{
    string fname1 = g_random()->Name("testfile", ".txt");
    string fname2 = g_random()->Name("testfile", ".txt");
    EXPECT_NE(fname1, fname2);
}



TEST_F(TestGRandom, templateSubstNSR319)
{
    int mean = 100;
    int sigma = 10;
    EXPECT_ANY_THROW( g_random()->Gauss<int>(mean, sigma));
}


TEST_F(TestGRandom, sanity_checks)
{
    EXPECT_EQ(42, g_random()->Uniform<int>(42, 42)); // A uniform variable distributed in the interval [42 42] must equal 42
    EXPECT_EQ(43, g_random()->Uniform<float>(43, 43)); // A uniform variable distributed in the interval [43 43] must equal 43
    EXPECT_EQ(44, g_random()->Uniform<float>(44, 44)); // A uniform variable distributed in the interval [44 44] must equal 44
    EXPECT_EQ(200, g_random()->Binominal<long>(200, 1)); // If we toss a coin 200 times with probability of success at each trial == one, then we should get 200 heads (or coins)
    EXPECT_EQ(200, g_random()->Binominal<long>(200, 1)); // If we toss a coin 300 times with probability of success at each trial == ZERO, then we should ZERO  heads (or coins)
}



TEST_F(TestGRandom, random_string)
{
    string r1 = g_random()->RandomString( 100 );
    string r2 = g_random()->RandomString( 100 );
    EXPECT_NE(r1, r2);
}