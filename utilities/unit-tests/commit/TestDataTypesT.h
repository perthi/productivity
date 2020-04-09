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

#include <utilities/GRandom.h>
#include <typeinfo>
#include <utilities/GDataTypes.h>
#include <testlib/TestBase.h>

#include <vector>

using std::vector;

/** Type paremetrized test for the the data type class. Ie all Data types that inherits form Val_t */
template <typename T>
class TestDataTypesT : public  TestBase
{
 public:
 TestDataTypesT() :  fPar1(0), fPar2(0), fPar3(0), fMin1(0), fMax1(0) {};
    virtual void SetUp();

protected:
    T fPar1;
    T fPar2;
    T fPar3;
    double fMin1 = 0;
    double fMax1 = 0;
};

bool checkOperator(const string op);

template< typename T1, typename T2  >
bool checkRange(const T1 par1, const T2 par2, const double comp);

template< typename T1, typename T2, typename T3 >
void testOperators(const T1 val1, const T2 val2, const T3 val3, const long double d1, const long double d2, const string op);

template <typename T1, typename T2, typename T3, typename T4 >
void operatorSanityCheck();


template<typename T>
void TestDataTypesT<T>::SetUp()
{
    #ifdef HAS_LOGGING
	SET_LOGTARGET("--target-off --target-file");
    #endif

    fMin1 = fPar1.GetMin();
    fMax1 = fPar1.GetMax();
}


#if GTEST_HAS_TYPED_TEST

TYPED_TEST_CASE_P(TestDataTypesT);



/** Testing boundary values. We expect an exception if we go one below the minimum value or one above max value.
*   Values exactly on the limit should be fine. */
TYPED_TEST_P(TestDataTypesT, Limits)
{
    
    EXPECT_ANY_THROW(this->fPar1 = this->fMax1 + 1);
    EXPECT_ANY_THROW(this->fPar1 = this->fMin1 - 1);
    EXPECT_NO_THROW(this->fPar1 = this->fMax1);
    EXPECT_NO_THROW(this->fPar1 = this->fMin1);
    EXPECT_ANY_THROW(this->fPar1.CheckLimits(this->fMax1 + 1, this->fMin1, this->fMax1) );
    
}



/** Testing that the datatypes behaves correctly and that we can assigned an arbitrary
value that is within the range of the variable */
TYPED_TEST_P(TestDataTypesT, Values)
{
    double num = g_random()->Uniform<double>(this->fMin1, this->fMax1);
    EXPECT_NO_THROW(this->fPar1 = num);
    EXPECT_DOUBLE_EQ(num, this->fPar1.GetValue());

    double num2 = g_random()->Uniform<double>( this->fMin1, this->fMax1);

    this->fPar1.SetValue(num2);
    EXPECT_DOUBLE_EQ(num2, this->fPar1.GetValue());

    double val1 = this->fPar1.GetValue();
   
    if (num2 + 1 < this->fPar1.GetMax())
    {
        this->fPar1 = this->fPar1 + 1;
        double val2 = this->fPar1.GetValue();
        EXPECT_DOUBLE_EQ(val2, val1 + 1);
    }
    
}



/** Here we want to verify that the basic operators  "+" "-" "*" "/" behaves as expected
*  if both operand and the result is inside range, and that an exception is thrown if they are outside of range
*  We test them on a somewhat arbritrary set of values */
TYPED_TEST_P(TestDataTypesT, Operators)
{
    /*
    vector<double> val1 = { 1,   2, 33,  4.5,  65,  112.3,  1000 };
    vector<double> val2 = { 3, -22, 12, 88.9,  65,  -2,  -220 };
    
    int64_t n = val1.size() > val2.size() ? val1.size() : val2.size();

    for (int i = 0; i < n; i++)
    {
        if (val1[i] > this->fMax1 || val1[i] < this->fMin1)
        {
            ASSERT_ANY_THROW( this->fPar1 = val1[i]);
        }
        else
            if (val2[i] > this->fMax1 || val2[i] < this->fMin1)
            {
                ASSERT_ANY_THROW( this->fPar2 = val2[i]);
            }
            else
            {
               this->fPar1 = val1[i];
               this->fPar2 = val2[i];
               testOperators( this->fPar1, this->fPar2, this->fPar3, val1[i], val2[i], "+");
               testOperators( this->fPar1, this->fPar2, this->fPar3, val1[i], val2[i], "-");
               testOperators( this->fPar1, this->fPar2, this->fPar3, val1[i], val2[i], "*");
               testOperators( this->fPar1, this->fPar2, this->fPar3, val1[i], val2[i], "/");
               EXPECT_FALSE( checkOperator ("blahh") );
            }
    }
    */
}



/** Here we reigister testst for the TestDataTypesP parametrized test class
 *  In order to run parametrized test the following 3 things must be done 
 *  1) Register test cases - That is done with the  REGISTER_TYPED_TEST_CASE_P macro as shown below
 *  2) Make a list of types you want to run the tests for for instance something like  
 *     using testing::Types;
 *     typedef Types< T1  , T2,  t3, ....... atc > impl;  (See TestKDataTypes.cpp / h for examples )
 *  3) Instantiate a typed test case using the INSTANTIATE_TYPED_TEST_CASE_P  macro i.e
 *     INSTANTIATE_TYPED_TEST_CASE_P("Test Case", ClasssName, impl); (See TestKDataTypes.cpp / h for examples )
 *  
 *  Only tetst registered with the REGISTER_TYPED_TEST_CASE_P macro will actually be performed by the Google test 
 *  framework (The others will still be compiled).
**/
REGISTER_TYPED_TEST_CASE_P(TestDataTypesT, Limits, Values, Operators);


/** Check if an oprator is any of the four valid ones, "+", "-", "*", "/"
* @return true for a valid operator, false othervise */
bool checkOperator(const string op)
{
    vector<string> valid = { "+", "-", "*", "/" };
    for (unsigned int i = 0; i < valid.size(); i++)
    {
        if (op == valid[i])
        {
            return true;
        }
    }
    return false;
}


/** Helper function. Check if the value "comp" is within range of both par1, and par2
* That is, that "comp" is less than the max value of both par1, and par2 and more than both par1 and par2
* @tparam T1 Type of first parameter
* @tparam T2 Type of second parameter
* @tparam par1 Lower limite
* @tparam par2  Upper limit
* @param comp  The value to check
* @return true if "comp is within range of par1 and par2 and false othervise" */
template< typename T1, typename T2  >
bool checkRange(const T1 par1, const T2 par2, const double compare )
{
    double max1 = par1.GetMax();
    double max2 = par2.GetMax();
    double min1 = par1.GetMin();
    double min2 = par2.GetMin();

    if (compare > max1 || compare  < min1 || compare  > max2 || compare  < min2)
    {
        return false;
    }
    else
    {
        return true;
    }
}



/** Testing of binary operators
*   @tparam T1    data type for left operand
*   @tparam T2    data type for right operand
*   @tparam T3    data type for the return value
*   @param  val1  value of left operand
*   @param  val2  value right operand
*   @param  val3  The return value
*   @param  d1    arbritrary double value to use for operator check
*   @param  d2    arbritrary double value to use for operator check
*   @param  op    The operator to test, must be any of {+, -, *, / }
*   The purpose of "d1" and "d2" is to test the regular +, -, * and / opearators against the corresponding operators
*   for the user defined types that inherits from Val_t. We use long double for this since all operations on long doubles is also valid
*   for short, int, long int, float etc... */
template< typename T1, typename T2, typename T3 >
void testOperators(const T1 val1, const T2 val2,  T3 val3, const double d1, const double d2, const string op)
{
    EXPECT_TRUE(checkOperator(op));
    double expected;

    if (op == "+")
    {
        expected = d1 + d2;
        if (checkRange(val1, val2, expected) == false)
        {
            EXPECT_ANY_THROW(val3 = val1 + val2);
        }
        else
        {
            val3 = val1 + val2;
            EXPECT_EQ(expected, val3.GetValue());
        }
    }

    if (op == "-")
    {
        expected = d1 - d2;
        if (checkRange(val1, val2, expected) == false)
        {
            EXPECT_ANY_THROW(val3 = val1 - val2);
        }
        else
        {
            val3 = val1 - val2;
            EXPECT_EQ(expected, val3.GetValue());
        }
    }

    if (op == "*")
    {
        expected = d1 * d2;
        if (checkRange(val1, val2, expected) == false)
        {
           EXPECT_ANY_THROW(val3 = val1*val2);
        }
        else
        {
            val3 = val1*val2;
            EXPECT_EQ(expected, val3.GetValue());
        }
    }

    if (op == "/")
    {
        expected = d1 / d2;
        if (checkRange(val1, val2, expected) == false)
        {
            EXPECT_ANY_THROW(val3 = val1 / val2);
        }
        else
        {
            val3 = val1 / val2;
            EXPECT_EQ(expected, val3.GetValue());
        }
    }
}


/** Testing of the four operators using different datatypes. We want to allow multiplication and division between different types, but we disallow
*   addition and subtraction. For instance  adding meters and seconds does not make sens at is almost certainly a user error)
*   Before we start we just make sure that if we get an exception then its the operator / types that is the problem, and not that we are out of range, *
*   we dont care about the actual values of the type parameters for this test
*  @tparam T1 first  template parameter
*  @tparam T2 second template parameter
*  @tparam T3 third templat parameter
*  @tparam T4 fourth templat parameter */
template <typename T1, typename T2, typename T3, typename T4 >
void operatorSanityCheck()
{
  //  SET_LOGLEVEL("--all-off");
   // SET_LOGTARGET( eMSGTARGET::TARGET_FILE);
  //   SET_LOGTARGET( "--target-file");

    vector<T1> t1(4);
    vector<T2> t2(3);
    T3 t3;
    T4 t4;
    setLimits(t1, -100, 100);
    setLimits(t2, -100, 100);
    t3.SetLimits(-100, 100);
    t4.SetLimits(-100, 100);

    t1[0] = 1.0;
    t1[1] = 2.0;
    t1[2] = 3.0;
    t1[3] = 0.0;
    t2[0] = t2[1] = t2[2] = 0.0;
    t3 = 0.0;
    t4 = 0.0;



    EXPECT_ANY_THROW(t1[0] + t3);

    EXPECT_ANY_THROW(t4 = t2[0] + t3);
    EXPECT_ANY_THROW(t4 = t2[0] - t4);
 //   EXPECT_ANY_THROW(t2[0] + t3);
 //   EXPECT_ANY_THROW(t2[0] - t4);

    EXPECT_NO_THROW(t2[2] = t2[0] + t2[1]);
    EXPECT_NO_THROW(t2[0] + t2[1]);

    EXPECT_ANY_THROW(t1[0] - t3);
 
    EXPECT_NO_THROW(t2[2] = t2[0] - t2[1]);
    EXPECT_NO_THROW(t2[0] - t2[1]);

    EXPECT_ANY_THROW(t1[2] / t3);
    EXPECT_ANY_THROW(t2[0] / t4);

    EXPECT_NO_THROW(t2[2] * t3);
    EXPECT_NO_THROW(t1[0] * t4);

#ifdef _WIN32
#pragma warning(suppress: 4244)  // It is a part of the test to assign double to int 
    EXPECT_NO_THROW(t1[3] = t1[0] * t1[1] * t1[2]);
#else
    EXPECT_NO_THROW(t1[3] = t1[0] * t1[1] * t1[2]);
#endif
    double expected = 1 * 2 * 3; // = 6
    EXPECT_EQ(expected, t1[3].GetValue());
}


#endif
