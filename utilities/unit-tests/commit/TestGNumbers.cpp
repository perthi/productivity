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

#include "TestGNumbers.h"
#include <utilities/GNumbers.h>
#include <utilities/GRandom.h>

#ifdef HAS_LOGGING
#include <logging/LMessageGenerator.h>
#endif


TEST_F(TestGNumbers, count_ones)
{
	EXPECT_EQ( 0, g_numbers()->CountBits(0) );
	EXPECT_EQ( 1, g_numbers()->CountBits(0x1) );
	EXPECT_EQ( 1, g_numbers()->CountBits(0x2) );
	EXPECT_EQ( 2, g_numbers()->CountBits(0x3) );
	EXPECT_EQ( 5, g_numbers()->CountBits(181) );
	EXPECT_EQ( 9, g_numbers()->CountBits(14579) );
}


TEST_F(TestGNumbers, Dec2Hex)
{
    EXPECT_EQ( "0xa", g_numbers()->Dec2Hex("10") );
    EXPECT_EQ("0xabcdef", g_numbers()->Dec2Hex("11259375"));
    EXPECT_ANY_THROW( g_numbers()->Dec2Hex("notanumber") );
    EXPECT_ANY_THROW(g_numbers()->Dec2Hex("0xabc"));
}


TEST_F(TestGNumbers, Hex2Dec)
{
    EXPECT_EQ("11259375", g_numbers()->Hex2Dec("0xabcdef"));
    EXPECT_EQ("10", g_numbers()->Hex2Dec("0xa"));
    EXPECT_ANY_THROW(g_numbers()->Hex2Dec("notanumber"));
    EXPECT_ANY_THROW(g_numbers()->Hex2Dec("2748"));
}


TEST_F(TestGNumbers, Hex2DecDec2Hex)
{
    EXPECT_EQ("11259375", g_numbers()->Hex2Dec(g_numbers()->Dec2Hex("11259375")));
    EXPECT_EQ("300", g_numbers()->Hex2Dec(g_numbers()->Dec2Hex("300")));
    EXPECT_EQ("123456789", g_numbers()->Hex2Dec(g_numbers()->Dec2Hex("123456789")));

    EXPECT_EQ("0x300", g_numbers()->Dec2Hex(g_numbers()->Hex2Dec("0x300")));
    EXPECT_EQ("0xdead", g_numbers()->Dec2Hex(g_numbers()->Hex2Dec("0xdead")));
    EXPECT_EQ("0xdeaddead", g_numbers()->Dec2Hex(g_numbers()->Hex2Dec("0xdeaddead")));
}


TEST_F(TestGNumbers, IsAlphaNumber)
{
   EXPECT_TRUE( g_numbers()->IsAlphaNumber("A")); 
   EXPECT_TRUE(g_numbers()->IsAlphaNumber("Q"));
   EXPECT_TRUE(g_numbers()->IsAlphaNumber("ABC123"));
   EXPECT_TRUE(g_numbers()->IsAlphaNumber("QXC678"));
   EXPECT_TRUE(g_numbers()->IsAlphaNumber("    QXC678"));
   EXPECT_TRUE(g_numbers()->IsAlphaNumber("    QXC678    "));
   EXPECT_TRUE(g_numbers()->IsAlphaNumber("QXC678    "));
   EXPECT_FALSE(g_numbers()->IsAlphaNumber("�"));
   EXPECT_FALSE(g_numbers()->IsAlphaNumber("�"));
   EXPECT_FALSE(g_numbers()->IsAlphaNumber(" ���wxz  "));
   EXPECT_FALSE(g_numbers()->IsAlphaNumber(" � � � �� � 123 345  "));
}


TEST_F(TestGNumbers, IsBinary)
{
    EXPECT_TRUE(g_numbers()->IsBinary("0"));
    EXPECT_TRUE(g_numbers()->IsBinary("1"));
    EXPECT_TRUE(g_numbers()->IsBinary("110010110"));
    EXPECT_TRUE(g_numbers()->IsBinary("   110010110"));
    EXPECT_TRUE(g_numbers()->IsBinary("110010110   "));
    EXPECT_TRUE(g_numbers()->IsBinary("     110010110  "));
    EXPECT_TRUE(g_numbers()->IsBinary("1100101101111111100000111101101010"));
    EXPECT_FALSE(g_numbers()->IsBinary("110210110"));
    EXPECT_TRUE(g_numbers()->IsBinary( string("0")));
    EXPECT_TRUE(g_numbers()->IsBinary( string("1")));
    EXPECT_TRUE(g_numbers()->IsBinary( string("110010110")));
    EXPECT_TRUE(g_numbers()->IsBinary( string("   110010110")));
    EXPECT_TRUE(g_numbers()->IsBinary( string("110010110   ")));
    EXPECT_TRUE(g_numbers()->IsBinary( string("     110010110  ")));
    EXPECT_TRUE(g_numbers()->IsBinary( string("1100101101111111100000111101101010")));
    EXPECT_FALSE(g_numbers()->IsBinary(string("110210110")));
}




TEST_F(TestGNumbers, ToBinary)
{
    EXPECT_EQ(457893, g_numbers()->ToBinary("1101111110010100101"));
    EXPECT_EQ(0, g_numbers()->ToBinary("0"));
    EXPECT_EQ(1, g_numbers()->ToBinary("1"));
    EXPECT_EQ(15, g_numbers()->ToBinary("1111"));
    EXPECT_EQ(6, g_numbers()->ToBinary("110"));
    EXPECT_EQ(15, g_numbers()->ToBinary("  1111"));
    EXPECT_EQ(6, g_numbers()->ToBinary("110  "));
    EXPECT_EQ(6, g_numbers()->ToBinary("   110     "));
    EXPECT_EQ(123456, g_numbers()->ToBinary("11110001001000000") );
    
    #ifdef _WIN32
    EXPECT_EQ(9223372036854775807, g_numbers()->ToBinary("  111111111111111111111111111111111111111111111111111111111111111"));
    EXPECT_EQ(-9223372036854775807, g_numbers()->ToBinary("-111111111111111111111111111111111111111111111111111111111111111"));
    #endif

    EXPECT_EQ(-7, g_numbers()->ToBinary("-111"));
    EXPECT_EQ(-6, g_numbers()->ToBinary("-110"));
    EXPECT_ANY_THROW(g_numbers()->ToBinary("12345"));
}




TEST_F(TestGNumbers, IsDecNumber)
{
    EXPECT_TRUE( g_numbers()->IsDecNumber("10") );
    EXPECT_TRUE(g_numbers()->IsDecNumber("9223372036854775807"));
    EXPECT_FALSE(g_numbers()->IsDecNumber("0x10"));
    EXPECT_FALSE(g_numbers()->IsDecNumber("blahhhhh"));
}



TEST_F(TestGNumbers, IsInteger)
{
    EXPECT_TRUE( g_numbers()->IsInteger("9223372036854775807"));
    EXPECT_TRUE(g_numbers()->IsInteger("-9223372036854775807"));
    EXPECT_TRUE(g_numbers()->IsInteger("123456789101112131"));
    EXPECT_TRUE(g_numbers()->IsInteger(string("9223372036854775807")));
    EXPECT_TRUE(g_numbers()->IsInteger(string("-9223372036854775807")));
    EXPECT_TRUE(g_numbers()->IsInteger(string("123456789101112131")));
    EXPECT_TRUE(g_numbers()->IsInteger( 9223372036854775807));
    EXPECT_TRUE(g_numbers()->IsInteger(-9223372036854775807));
    EXPECT_TRUE(g_numbers()->IsInteger(123456789101112131));
    EXPECT_FALSE(g_numbers()->IsInteger(1.5));
    EXPECT_FALSE(g_numbers()->IsInteger(1.61803398875));
    EXPECT_TRUE( g_numbers()->IsInteger("1"));
    EXPECT_TRUE(g_numbers()->IsInteger("01"));
    EXPECT_TRUE(g_numbers()->IsInteger("9"));
    EXPECT_TRUE(g_numbers()->IsInteger(" 9"));
    EXPECT_TRUE(g_numbers()->IsInteger("     9  "));
    EXPECT_TRUE(g_numbers()->IsInteger("07"));
    EXPECT_TRUE(g_numbers()->IsInteger("1234567"));
    EXPECT_TRUE(g_numbers()->IsInteger("     1234567"));
    EXPECT_TRUE(g_numbers()->IsInteger("000001234567"));
    EXPECT_TRUE(g_numbers()->IsInteger("77  "));
    EXPECT_TRUE(g_numbers()->IsInteger("  77  "));
    EXPECT_TRUE(g_numbers()->IsInteger("77"));
    EXPECT_FALSE(g_numbers()->IsInteger("1 2"));
    EXPECT_FALSE(g_numbers()->IsInteger("1 2 33 55"));
    EXPECT_FALSE(g_numbers()->IsInteger("Meeny"));
    EXPECT_FALSE(g_numbers()->IsInteger("1.33"));
}



TEST_F(TestGNumbers, IsNumber)
{
    EXPECT_TRUE(  g_numbers()->IsNumber("77  ") );
    EXPECT_TRUE(  g_numbers()->IsNumber("1.33") );
    EXPECT_TRUE(  g_numbers()->IsNumber("QXC678") );
    EXPECT_TRUE(  g_numbers()->IsNumber("0xabcd"));
    EXPECT_TRUE(  g_numbers()->IsNumber("   0xdef"));
	EXPECT_TRUE(g_numbers()->IsNumber("   0xdead   "));
	EXPECT_TRUE(g_numbers()->IsNumber("   NaN   "));
	EXPECT_FALSE( g_numbers()->IsNumber("lorem ipsum") );

    string tmp = "   0xffff   ";
    EXPECT_TRUE(g_numbers()->IsNumber(tmp));

    EXPECT_TRUE(g_numbers()->IsNumber("10000"));

}



TEST_F(TestGNumbers, NSR1033)
{
    EXPECT_TRUE(g_numbers()->IsNumber("10000\n"));
    EXPECT_TRUE(g_numbers()->IsNumber("10000\t\t"));
    EXPECT_TRUE(g_numbers()->IsNumber("10000 \t\t \n"));
    EXPECT_TRUE(g_numbers()->IsNumber("\t10000 \t\t \n"));
    EXPECT_TRUE(g_numbers()->IsNumber("\t\n10000 \t\t \n"));
    EXPECT_TRUE(g_numbers()->IsNumber("\t \n 10000 \t\t \n"));
    EXPECT_TRUE(g_numbers()->IsInteger("1234567 \t \n"));
    EXPECT_TRUE(g_numbers()->IsAlphaNumber("QXC678   \n "));
    EXPECT_TRUE(g_numbers()->IsFloat("3.14159265359\t"));
    EXPECT_TRUE(g_numbers()->IsFloat("3.14159265359\n"));
    EXPECT_TRUE(g_numbers()->IsFloat("3.14159265359\t\n"));
    EXPECT_DOUBLE_EQ(1.7976931348623158e+308, g_numbers()->ToFloat("1.7976931348623158e+308\n"));
    vector<string> int_array = { "0xa\n", "0Xabc\t", "0x12bc  ", "123     \n" };
    vector<long long int> num = g_numbers()->ToInteger< long long int >(int_array);
    EXPECT_EQ(0xa, num[0]);
    EXPECT_EQ(0xabc, num[1]);
    EXPECT_EQ(0x12bc, num[2]);
    EXPECT_EQ(123, num[3]);
}


TEST_F(TestGNumbers, ToNumber)
{
    EXPECT_EQ(9223372036854775807, g_numbers()->ToNumber<long long int>("9223372036854775807"));
    EXPECT_DOUBLE_EQ(1.7676931348623158e+308, g_numbers()->ToNumber<long double>("1.7676931348623158e+308\n"));
}

TEST_F(TestGNumbers, ToInteger)
{
    EXPECT_EQ(9223372036854775807,  g_numbers()->ToInteger("9223372036854775807") );
    EXPECT_EQ(-9223372036854775807, g_numbers()->ToInteger("-9223372036854775807"));
    EXPECT_EQ(0xabcdef, g_numbers()->ToInteger("0xabcdef"));
    vector<string> int_array = {"0xa", "0Xb", "0x12bc", "123"};
    vector<long long int> num = g_numbers()->ToInteger< long long int >(int_array);
    EXPECT_EQ(0xa, num[0]);
    EXPECT_EQ(0xb, num[1]);
    EXPECT_EQ(0x12bc, num[2]);
    EXPECT_EQ(123, num[3]);
}



TEST_F(TestGNumbers, IsFloat)
{
    EXPECT_TRUE(g_numbers()->IsFloat("123456789101112131"));
    EXPECT_FALSE(g_numbers()->IsInteger("3.14"));
    EXPECT_FALSE(g_numbers()->IsInteger("   3.14  "));
    EXPECT_TRUE(g_numbers()->IsFloat("3.14159265359"));
    EXPECT_TRUE(g_numbers()->IsFloat("      3.14159265359"));
    EXPECT_TRUE(g_numbers()->IsFloat("3.14159265359      "));
    EXPECT_TRUE(g_numbers()->IsFloat("   3.14159265359   "));
    EXPECT_TRUE(g_numbers()->IsFloat("-3.14159265359      "));
    EXPECT_TRUE(g_numbers()->IsFloat("   -3.14159265359   "));
	EXPECT_TRUE(g_numbers()->IsFloat("nan"));

    string goldencut = "  1.618033989   ";
    EXPECT_TRUE(g_numbers()->IsFloat(goldencut));
    EXPECT_TRUE(g_numbers()->IsFloat(goldencut.c_str() ));

    EXPECT_TRUE( g_numbers()->IsFloat("1.7976931348623158e+308" ));
    EXPECT_TRUE( g_numbers()->IsFloat( string("123456789101112131")));
    EXPECT_FALSE(g_numbers()->IsInteger( string("3.14")) );
    EXPECT_FALSE(g_numbers()->IsInteger( string("   3.14  ")));
    EXPECT_TRUE(g_numbers()->IsFloat( string("3.14159265359") ));
    EXPECT_TRUE(g_numbers()->IsFloat( string("      3.14159265359")));
    EXPECT_TRUE(g_numbers()->IsFloat( string("3.14159265359      ")));
    EXPECT_TRUE(g_numbers()->IsFloat( string("   3.14159265359   ")));
    EXPECT_TRUE(g_numbers()->IsFloat( string("-3.14159265359      ")));
	EXPECT_TRUE(g_numbers()->IsFloat(string("   -3.14159265359   ")));
///	EXPECT_TRUE(g_numbers()->IsFloat(string("   -3,14159265359   ")));
	EXPECT_TRUE(g_numbers()->IsFloat(string( "1.7976931348623158e+308")));
    EXPECT_TRUE(g_numbers()->IsFloat(1.61803398875));
    EXPECT_TRUE(g_numbers()->IsFloat(-1.61803398875));
    EXPECT_TRUE(g_numbers()->IsFloat(9223372036854775807));
    EXPECT_TRUE(g_numbers()->IsFloat(-1));
    EXPECT_TRUE(g_numbers()->IsFloat( 1.7976931348623158e+308));
}



TEST_F(TestGNumbers, IsType)
{
    float   f1 = 0;
    double  f2 = 0;
    long  double f3 = 0;
    short i1 = 0;
    int i2 = 0;
    long int i3 = 0;
    long long int i4 = 0;
    unsigned short i5 = 0;
    unsigned  int i6 = 0;
    unsigned long int i7 = 0;
    unsigned  long long int i8 = 0;


    EXPECT_FALSE(g_numbers()->IsUnsignedType(f1));
    EXPECT_FALSE(g_numbers()->IsUnsignedType(f2));
    EXPECT_FALSE(g_numbers()->IsUnsignedType(f3));

    EXPECT_FALSE(g_numbers()->IsUnsignedType(i1));
    EXPECT_FALSE(g_numbers()->IsUnsignedType(i2));
    EXPECT_FALSE(g_numbers()->IsUnsignedType(i3));
    EXPECT_FALSE(g_numbers()->IsUnsignedType(i4));
    EXPECT_TRUE(g_numbers()->IsUnsignedType(i5));
    EXPECT_TRUE(g_numbers()->IsUnsignedType(i6));
    EXPECT_TRUE(g_numbers()->IsUnsignedType(i7));
    EXPECT_TRUE(g_numbers()->IsUnsignedType(i8));

   
    EXPECT_TRUE(g_numbers()->IsFloatType(f1));
    EXPECT_TRUE(g_numbers()->IsFloatType(f2));
    EXPECT_TRUE(g_numbers()->IsFloatType(f3));
    
    EXPECT_FALSE(g_numbers()->IsFloatType(i1));
    EXPECT_FALSE(g_numbers()->IsFloatType(i2));
    EXPECT_FALSE(g_numbers()->IsFloatType(i3));
    EXPECT_FALSE(g_numbers()->IsFloatType(i4));
    EXPECT_FALSE(g_numbers()->IsFloatType(i5));
    EXPECT_FALSE(g_numbers()->IsFloatType(i6));
    EXPECT_FALSE(g_numbers()->IsFloatType(i7));
    EXPECT_FALSE(g_numbers()->IsFloatType(i8));

    EXPECT_TRUE(g_numbers()->IsFloatType(100.0));
    EXPECT_FALSE(g_numbers()->IsFloatType(100));
    
    EXPECT_FALSE(g_numbers()->IsIntegerType(f1));
    EXPECT_FALSE(g_numbers()->IsIntegerType(f2));
    EXPECT_TRUE(g_numbers()->IsFloatType(f3));


    EXPECT_TRUE(g_numbers()->IsIntegerType(i1));
    EXPECT_TRUE(g_numbers()->IsIntegerType(i2));
    EXPECT_TRUE(g_numbers()->IsIntegerType(i3));
    EXPECT_TRUE(g_numbers()->IsIntegerType(i4));
    EXPECT_TRUE(g_numbers()->IsIntegerType(i5));
    EXPECT_TRUE(g_numbers()->IsIntegerType(i6));
    EXPECT_TRUE(g_numbers()->IsIntegerType(i7));
    EXPECT_TRUE(g_numbers()->IsIntegerType(i8));
    EXPECT_FALSE(g_numbers()->IsFloatType(string("0xabcd")));
}



TEST_F(TestGNumbers, IsFundamentalType)
{
    int i = 0;
    EXPECT_TRUE(g_numbers()->IsFundamentalType(i));
    string t = typeid(i).name();
	EXPECT_TRUE(g_numbers()->IsFundamentalTypeS(string(t.c_str())));
}



TEST_F(TestGNumbers, IsVType)
{
    vector<char>          vi1;
    vector<short>         vi2;
    vector<int>           vi3;
    vector<long int>      vi4;
    vector<long long int> vi5;
    vector<unsigned char>     vi6;
    vector<unsigned short>    vi7;
    vector<unsigned int>      vi8;
    vector<unsigned long int> vi9;
    vector<unsigned long long int> vi10;
    vector<float>   vf1;
    vector<double>  vf2;
    vector<long double>  vf3;

    EXPECT_TRUE(g_numbers()->IsIntegerVType(vi1));
    EXPECT_TRUE(g_numbers()->IsIntegerVType(vi2));
    EXPECT_TRUE(g_numbers()->IsIntegerVType(vi3));
    EXPECT_TRUE(g_numbers()->IsIntegerVType(vi4));
    EXPECT_TRUE(g_numbers()->IsIntegerVType(vi5));
    EXPECT_TRUE(g_numbers()->IsIntegerVType(vi6));
    EXPECT_TRUE(g_numbers()->IsIntegerVType(vi7));
    EXPECT_TRUE(g_numbers()->IsIntegerVType(vi8));
    EXPECT_TRUE(g_numbers()->IsIntegerVType(vi9));
    EXPECT_FALSE(g_numbers()->IsIntegerVType(vf1));
    EXPECT_FALSE(g_numbers()->IsIntegerVType(vf2));
    EXPECT_FALSE(g_numbers()->IsIntegerVType(vf3));

    EXPECT_FALSE(g_numbers()->IsFloatVType(vi1));
    EXPECT_FALSE(g_numbers()->IsFloatVType(vi2));
    EXPECT_FALSE(g_numbers()->IsFloatVType(vi3));
    EXPECT_FALSE(g_numbers()->IsFloatVType(vi4));
    EXPECT_FALSE(g_numbers()->IsFloatVType(vi5));
    EXPECT_FALSE(g_numbers()->IsFloatVType(vi6));
    EXPECT_FALSE(g_numbers()->IsFloatVType(vi7));
    EXPECT_FALSE(g_numbers()->IsFloatVType(vi8));
    EXPECT_FALSE(g_numbers()->IsFloatVType(vi9));
    EXPECT_TRUE(g_numbers()->IsFloatVType(vf1));
    EXPECT_TRUE(g_numbers()->IsFloatVType(vf2));
    EXPECT_TRUE(g_numbers()->IsFloatVType(vf3));

    EXPECT_TRUE(g_numbers()->IsFundamentalVType(vi1));
    EXPECT_TRUE(g_numbers()->IsFundamentalVType(vi2));
    EXPECT_TRUE(g_numbers()->IsFundamentalVType(vi3));
    EXPECT_TRUE(g_numbers()->IsFundamentalVType(vi4));
    EXPECT_TRUE(g_numbers()->IsFundamentalVType(vi5));
    EXPECT_TRUE(g_numbers()->IsFundamentalVType(vi6));
    EXPECT_TRUE(g_numbers()->IsFundamentalVType(vi7));
    EXPECT_TRUE(g_numbers()->IsFundamentalVType(vi8));
    EXPECT_TRUE(g_numbers()->IsFundamentalVType(vi9));
    EXPECT_TRUE(g_numbers()->IsFundamentalVType(vf1));
    EXPECT_TRUE(g_numbers()->IsFundamentalVType(vf2));
    EXPECT_TRUE(g_numbers()->IsFundamentalVType(vf3));
}


TEST_F(TestGNumbers, ToFloat)
{
	EXPECT_DOUBLE_EQ( 1.7976931348623158e+308, g_numbers()->ToFloat("1.7976931348623158e+308"));
    EXPECT_DOUBLE_EQ( -1.7976931348623158e+308, g_numbers()->ToFloat("-1.7976931348623158e+308"));
    EXPECT_DOUBLE_EQ(3.14159265359,  g_numbers()->ToFloat("      3.14159265359"));
	EXPECT_DOUBLE_EQ(3.14159265359,  g_numbers()->ToFloat("3.14159265359      "));
	EXPECT_DOUBLE_EQ(3.14159265359, g_numbers()->ToFloat("   3.14159265359   "));
	EXPECT_DOUBLE_EQ(3.14159265359, g_numbers()->ToFloat("   3,14159265359   "));
	
	try
	{
		g_numbers()->ToFloat("   3,14159265359   ");
	}
    #ifdef HAS_LOGGING
	catch (GException &e)
	{
		CERR << e.what() << endl;
	}
    #endif
	catch(std::exception &e)
	{
		CERR << e.what() << endl;
	}
	catch (std::string s)
	{
		CERR << s << endl;
	}
	catch (...)
	{
		CERR << "Unknown exception caught" << endl;
	}


    EXPECT_DOUBLE_EQ((long double)9223372036854775807, g_numbers()->ToFloat("9223372036854775807"));
    EXPECT_DOUBLE_EQ((long double)-9223372036854775807, g_numbers()->ToFloat("-9223372036854775807"));
}



TEST_F(TestGNumbers, IsDigit)
{
   
    EXPECT_TRUE(g_numbers()->IsDigit(0));
    EXPECT_FALSE(g_numbers()->IsDigit("42"));
    EXPECT_FALSE(g_numbers()->IsDigit(" 42"));
    EXPECT_FALSE(g_numbers()->IsDigit(" 42   "));
    EXPECT_FALSE(g_numbers()->IsDigit("0.5"));
    EXPECT_FALSE(g_numbers()->IsDigit("3.14159265359"));

    EXPECT_TRUE(g_numbers()->IsDigit("4"));
    EXPECT_TRUE(g_numbers()->IsDigit(" 4"));
    EXPECT_TRUE(g_numbers()->IsDigit(" 4   "));
    EXPECT_FALSE(g_numbers()->IsDigit("x"));
    EXPECT_FALSE(g_numbers()->IsDigit("xyz"));
    EXPECT_FALSE(g_numbers()->IsDigit("lorem ipsum"));

    EXPECT_FALSE(g_numbers()->IsDigit(42));
    EXPECT_FALSE(g_numbers()->IsDigit(1.2345));
    EXPECT_FALSE(g_numbers()->IsDigit(3.14159265359) );

    short ri =  g_random()->Uniform<short>(10, 100);
    double rd = g_random()->Uniform<double>(10, 100);

    EXPECT_FALSE( g_numbers()->IsDigit(ri)); 
    EXPECT_FALSE(g_numbers()->IsDigit(rd));

    for (int i = 10; i < 100; i++)
    {
        EXPECT_FALSE(g_numbers()->IsDigit(i));
    }
 
  //  Test some other bases also, for instance base 2, 8, 16
    vector<int> bases = { BASE_2, BASE_8, BASE_10, BASE_16 };
    for (unsigned int i = 0; i < bases.size(); i++)
    {
        for (int j = 0; j < bases[i]; j++)
        {
            EXPECT_TRUE(g_numbers()->IsDigit(j, bases[i]));
        }
    }
    
    EXPECT_TRUE(  g_numbers()->IsDigit("1", BASE_2)); 
    EXPECT_TRUE(  g_numbers()->IsDigit("0", BASE_2));
    EXPECT_FALSE( g_numbers()->IsDigit("3", BASE_2));
    EXPECT_FALSE( g_numbers()->IsDigit("6", BASE_2));
    EXPECT_FALSE( g_numbers()->IsDigit("9", BASE_2));

    EXPECT_TRUE(g_numbers()->IsDigit("3", BASE_8));
    EXPECT_TRUE(g_numbers()->IsDigit("7", BASE_8));
    EXPECT_FALSE(g_numbers()->IsDigit("8", BASE_8));
    EXPECT_FALSE(g_numbers()->IsDigit("9", BASE_8));
    EXPECT_FALSE(g_numbers()->IsDigit("f", BASE_8));

    EXPECT_TRUE( g_numbers()->IsDigit("1", BASE_16));
    EXPECT_TRUE( g_numbers()->IsDigit("7", BASE_16));
    EXPECT_TRUE( g_numbers()->IsDigit("d", BASE_16));
    EXPECT_TRUE( g_numbers()->IsDigit("e", BASE_16));
    EXPECT_TRUE( g_numbers()->IsDigit("f", BASE_16));
    EXPECT_TRUE(g_numbers()->IsDigit("f", BASE_16));
  }
  



TEST_F(TestGNumbers, IsHex) 
{
  // EXPECT_TRUE(g_numbers()->IsHex("0xffffff\n"));
   EXPECT_TRUE(g_numbers()->IsHex("0x0"));
   EXPECT_FALSE(g_numbers()->IsHex("0x"));
   EXPECT_TRUE(g_numbers()->IsHex("0xabcd"));
   EXPECT_TRUE(g_numbers()->IsHex("   0xabcd"));
   EXPECT_TRUE(g_numbers()->IsHex("0Xabcd"));
   EXPECT_TRUE(g_numbers()->IsHex("   0Xabcd"));
   string test = "0x123abc";
   EXPECT_TRUE(g_numbers()->IsHex(test));
   EXPECT_TRUE(g_numbers()->IsHex(test.c_str() ));
   EXPECT_TRUE(g_numbers()->IsHex("0xabcd    "));
   EXPECT_TRUE(g_numbers()->IsHex("  0x12cf  "));
   EXPECT_TRUE(g_numbers()->IsHex("0x7FFFFFFFFFFFFFFF"));
   EXPECT_TRUE(g_numbers()->IsHex(string("0x0")));
   EXPECT_FALSE(g_numbers()->IsHex(string("0x")));
   EXPECT_TRUE(g_numbers()->IsHex(string("0xabcd")));
   EXPECT_TRUE(g_numbers()->IsHex(string("   0xabcd")));
   EXPECT_TRUE(g_numbers()->IsHex(string("0xabcd    ")));
   EXPECT_TRUE(g_numbers()->IsHex(string("  0x12cf  ")));
   EXPECT_TRUE(g_numbers()->IsHex(string("0x7FFFFFFFFFFFFFFF")));
   EXPECT_TRUE(g_numbers()->IsHex("-0x0"));
   EXPECT_FALSE(g_numbers()->IsHex("-0x"));
   EXPECT_TRUE(g_numbers()->IsHex("-0xabcd"));
   EXPECT_TRUE(g_numbers()->IsHex("   -0xabcd"));
   EXPECT_TRUE(g_numbers()->IsHex("-0xabcd    "));
   EXPECT_TRUE(g_numbers()->IsHex("  -0x12cf  "));
   EXPECT_TRUE(g_numbers()->IsHex("-0x7FFFFFFFFFFFFFFF"));
   EXPECT_TRUE(g_numbers()->IsHex(0x0));
   EXPECT_TRUE( g_numbers()->IsHex(0x3) );
   EXPECT_TRUE(g_numbers()->IsHex(100));
   EXPECT_TRUE(g_numbers()->IsHex(200));
   EXPECT_TRUE( g_numbers()->IsHex(0xabcd));
   EXPECT_TRUE(g_numbers()->IsHex(9223372036854775807));
   EXPECT_FALSE(g_numbers()->IsHex(1.4));
   EXPECT_FALSE(g_numbers()->IsHex(3.14));
   EXPECT_FALSE(g_numbers()->IsHex("abc0x4bc"));
   EXPECT_FALSE(g_numbers()->IsHex("lorem ipsum"));
   EXPECT_FALSE(g_numbers()->IsHex("Meeny"));
}



TEST_F(TestGNumbers, ToHex)
{
    EXPECT_EQ(9223372036854775807,  g_numbers()->ToHex("0x7FFFFFFFFFFFFFFF"));
    EXPECT_EQ(-9223372036854775807, g_numbers()->ToHex("-0x7FFFFFFFFFFFFFFF"));
    EXPECT_EQ( 0x7FFFFFFFFFFFFFFF,  g_numbers()->ToHex("0x7FFFFFFFFFFFFFFF"));
    EXPECT_EQ(-0x7FFFFFFFFFFFFFFF,  g_numbers()->ToHex("-0x7FFFFFFFFFFFFFFF"));
    EXPECT_EQ(-0xadef123, g_numbers()->ToHex("-0xadef123"));
    EXPECT_EQ(0xadef123, g_numbers()->ToHex("0xadef123"));
    EXPECT_EQ(-0xadef123, g_numbers()->ToHex("     -0xadef123"));
    EXPECT_EQ(0xadef123, g_numbers()->ToHex("0xadef123"      ));
    EXPECT_EQ(-0xadef123, g_numbers()->ToHex("-0xadef123"));
    EXPECT_EQ(0xadef123, g_numbers()->ToHex(       "0xadef123"     ));
}



TEST_F(TestGNumbers, PrivateFunctions)
{
    EXPECT_ANY_THROW( g_numbers()->CheckUnsigned<uint16_t>(  "-1" ));
    EXPECT_ANY_THROW(g_numbers()->CheckUnsigned<unsigned short>("-66"));
    EXPECT_ANY_THROW(g_numbers()->CheckUnsigned<unsigned int>("-66"));
    EXPECT_ANY_THROW(g_numbers()->CheckUnsigned<unsigned long int>("-66"));
    EXPECT_ANY_THROW(g_numbers()->CheckUnsigned<unsigned long long>("-66"));

    EXPECT_NO_THROW(g_numbers()->CheckUnsigned<unsigned short>("66"));
    EXPECT_NO_THROW(g_numbers()->CheckUnsigned<unsigned int>("66"));
    EXPECT_NO_THROW(g_numbers()->CheckUnsigned<unsigned long int>("66"));
    EXPECT_NO_THROW(g_numbers()->CheckUnsigned<unsigned long long>("66"));

    EXPECT_NO_THROW(g_numbers()->CheckUnsigned<int>("-66"));
    EXPECT_NO_THROW(g_numbers()->CheckUnsigned<float>("-66"));
    EXPECT_NO_THROW(g_numbers()->CheckUnsigned<double>("-66"));

    EXPECT_ANY_THROW(g_numbers()->CheckUnsigned<uint16_t>("not a number"));
    EXPECT_ANY_THROW(g_numbers()->CheckUnsigned<int>("not a number"));
    EXPECT_ANY_THROW(g_numbers()->CheckUnsigned<long double>("not a number"));

    vector<string>  vec = { "0", "0x456bc", "-13", "72", "42", "300", "44","1066"};
    EXPECT_ANY_THROW(g_numbers()->CheckUnsigned<unsigned long>(vec));
    EXPECT_NO_THROW(g_numbers()->CheckUnsigned<int>(vec));
}



TEST_F(TestGNumbers, BitWidth)
{
    EXPECT_ANY_THROW( g_numbers()->BitWidth("notanumber"));
    EXPECT_ANY_THROW( g_numbers()->BitWidth("1234"));
    EXPECT_EQ(1,      g_numbers()->BitWidth("1"));
    EXPECT_EQ(3,      g_numbers()->BitWidth("111"));
    EXPECT_EQ(3, g_numbers()->BitWidth("0111"));
    EXPECT_EQ(3, g_numbers()->BitWidth("000111"));
    EXPECT_EQ(3, g_numbers()->BitWidth("00000000111"));
    EXPECT_EQ(5, g_numbers()->BitWidth("011100"));
    EXPECT_EQ(5, g_numbers()->BitWidth("00011100"));
    EXPECT_EQ(5, g_numbers()->BitWidth("0000000011100"));
}



TEST_F(TestGNumbers, PadOnes)
{
    int unpadded = 0x2D; // = 101101
    int padded = 0x3f;   // = 111111

    EXPECT_EQ(padded, g_numbers()->PadOnes(unpadded));
}



#ifdef HAS_LOGGING
TEST_F(TestGNumbers, NSR1988DisableError)
{	
    
	#undef G_STANDALONE
	EXPECT_ANY_THROW( g_numbers()->ToNumber<float>("blahhhh") );
	GException::DisableException();
	g_numbers()->DisableError();
	EXPECT_NO_THROW(  g_numbers()->ToNumber<float>("blahhhh") );
	auto m = LMessageGenerator::Instance()->GetLastMsg();
	g_numbers()->DisableError();
	EXPECT_NO_THROW(g_numbers()->ToNumber<float>("blahhhh"));
	m = LMessageGenerator::Instance()-> GetLastMsg();
	EXPECT_NO_THROW(g_numbers()->ToHex("blahhhh"));
	m = LMessageGenerator::Instance()->GetLastMsg();
	EXPECT_NO_THROW(g_numbers()->ToBinary("blahhhh"));
	m = LMessageGenerator::Instance()->GetLastMsg();
	g_numbers()->EnableError();
	GException::EnableException();
}
#endif


