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

#include "TestDataTypesT.h"


/** @defgroup datatype_test Structs for testing of the datatype library. 
*   Here we decalere a couple of tests classes (structs actually) in order to test basic operations such as 
*   addition, subtraction, multiplication and division. In particular we want to cross check that addition and 
*   subtraction of different types is disallowed, and that
*   we get an exception on such attempts, as this is almost certainly a user error. 
*   We would also like to check that the Get and Set function works as expected and that the max/min range check is enforced correctly
*  @{
*/
struct Time_t : public Val_t <double >
{
    Time_t (double n = 0) : Val_t<double>::Val_t(n, 0, 20, "Seconds", "s", "The time in seconds") {};
    
    virtual   Time_t & operator = (const Val_t rhs)
    {
        SetValue(rhs.GetValue());
        return *this;
    }
};


struct Distance_t : public Val_t <double >
{
    Distance_t(double n = 0) : Val_t<double>::Val_t(n, 0, 20, "Distance", "S", "The distance in meters") {};
    virtual   Distance_t & operator = (const Val_t rhs)
    {
        SetValue(rhs.GetValue());
        return *this;
    }
};


struct Mass_t : public Val_t <double >
{
    Mass_t(double n = 0) : Val_t<double>::Val_t(n, 0, 20, "Mass", "m", "The mass in kilograms") {};
    virtual   Mass_t & operator = (const Val_t rhs)
    {
        SetValue(rhs.GetValue());
        return *this;
    }
};


struct Dummy_t : public Val_t < double >
{
    Dummy_t(double n = 0) : Val_t<double>::Val_t(n, 0, 20, "", "", "") {};
    virtual Dummy_t & operator = (const Val_t rhs)
    {
        SetValue(rhs.GetValue());
        return *this;
    }
};
/** @} */



TEST( Memberfunctions,  GettersAndAssignement )
{
    Mass_t d1, d2;
    d1 = 12;
    EXPECT_EQ("Mass",  d1.GetName() );
    EXPECT_EQ("m",     d1.GetSubscript() );
    EXPECT_EQ("The mass in kilograms", d1.GetHelpText());
    EXPECT_EQ("12", d1.GetValueString() );
    EXPECT_STREQ("12" , d1.GetValueCString());

    d1.SetName("Lorem Ipsum");
    d1.SetSubscript("x");
    d1.SetHelpText("Lorem ipsum dolor sit amet");

    EXPECT_EQ("Lorem Ipsum", d1.GetName());
    EXPECT_EQ("x", d1.GetSubscript());
    EXPECT_EQ("Lorem ipsum dolor sit amet", d1.GetHelpText());

    EXPECT_EQ(20, d1.GetMax() );
    EXPECT_EQ(0, d1.GetMin() );

    d1.SetLimits(-90, 80);

    EXPECT_EQ(  80, d1.GetMax() );
    EXPECT_EQ( -90, d1.GetMin() );

    // Checking the copy constructor, in particular that we retain the values assigned
    // to d1, and not the default values
   
    Mass_t *d3 = new Mass_t(d1);
    EXPECT_EQ(80, d3->GetMax());
    EXPECT_EQ(-90, d3->GetMin());
    EXPECT_EQ("Lorem Ipsum", d3->GetName());
    EXPECT_EQ("x", d3->GetSubscript());
    EXPECT_EQ("Lorem ipsum dolor sit amet", d3->GetHelpText());
 
    // Check that the assignment operator works correctly
    // If its does then all values should be set back to the original ones
    d1 = d2;
    EXPECT_EQ("Mass", d1.GetName());
    EXPECT_EQ("m", d1.GetSubscript());
    EXPECT_EQ("The mass in kilograms", d1.GetHelpText());
    EXPECT_EQ(20, d1.GetMax());
    EXPECT_EQ(0, d1.GetMin());
    
    delete d3;
}




#if GTEST_HAS_TYPED_TEST

using testing::Types;


/* Instantiate 3 tests for 4 different data types, i.e 12 tests
 * This test apllies only to a given datatype at a time. */
typedef Types< Time_t, Distance_t, Mass_t, Dummy_t > impl;


/* Bug in google test !! it seems that calling INSTANTIATE_TYPED_TEST_CASE_P only for the first test, also instantiate them for the other also
*  we therofr only need to call it once for the same test. */
INSTANTIATE_TYPED_TEST_CASE_P(Operators, TestDataTypesT, impl);


/* We also want to check the operators applied between different data types. 
* For those tests we cannot use typed unit tests which only applies to a single class type. */
TEST(Datatypes, OperatorSanityCheck1)
{
//    SET_LOGTARGET("--target-off --target-file");
    operatorSanityCheck<Time_t, Distance_t, Mass_t, Dummy_t>();
}


/** We make sure that 
 * 1) Adding or subtracting different SI units are disallowed
 * 2) Adding or subtracting different SI units are allowed
 * 3) Multiplying or dividing different SI units are ok */
TEST(Operator, NSR796)
{
//    SET_LOGTARGET("--target-off --target-file");
    Distance_t d1, d2, d3;
    Mass_t m1, m2, m3;
    Time_t t1, t2, t3;
    
    EXPECT_ANY_THROW( m1  + d1 );       
    EXPECT_ANY_THROW( t1 +  d1 );       
    EXPECT_ANY_THROW( t1 =  m1 + d1 ); 
    EXPECT_ANY_THROW( m1 =  t1 + d1 );  
    EXPECT_NO_THROW(t3 = t1 + t2);
    EXPECT_NO_THROW(m3 = m1 + m2);
}


#endif

