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

#include "TestGUtilities.h"
#include <utilities/GUtilities.h>
#include <map>
#include <vector>
#include <logging/LMessageGenerator.h>
#include <utilities/GTokenizer.h>

using std::vector;

TestGUtilities::TestGUtilities() : TestBase()
{

}

TestGUtilities::~TestGUtilities()
{

}


TEST_F(TestGUtilities, CheckLimits)
{
    EXPECT_ANY_THROW( g_utilities()->CheckLimits( 3,1,2, "varname", __FILE__,  __LINE__, __FUNCTION__ ));
    EXPECT_ANY_THROW( g_utilities()->CheckLimits( 99, 100, 10000,  "varname", __FILE__, __LINE__, __FUNCTION__ ));
    EXPECT_NO_THROW(  g_utilities()->CheckLimits( 101, 100, 10000, "varname", __FILE__, __LINE__, __FUNCTION__ ));
    EXPECT_ANY_THROW( CHECKLIMITS(3, 1, 2, 0 ));
    EXPECT_ANY_THROW( CHECKLIMITS(99, 100, 10000, 0 ));
    EXPECT_NO_THROW(  CHECKLIMITS(101, 100, 10000, 0 ));
}


TEST_F(TestGUtilities, IsInRange)
{
  EXPECT_TRUE( g_utilities()->IsInRange(42, 1, 100));
  EXPECT_FALSE(g_utilities()->IsInRange(-1, 1, 100));
  EXPECT_FALSE(g_utilities()->IsInRange(101, 1, 100));
}

TEST_F(TestGUtilities, ConvertFromBitsToArray)
{
	unsigned char AA = 0xaa; //10101010
	const	std::vector<unsigned char> vConst{ 1, 0, 1, 0, 1, 0, 1, 0 };
			std::vector<unsigned char> vTest(8);
	EXPECT_TRUE(g_utilities()->Bits2Array(AA, vTest));
	EXPECT_EQ(vConst, vTest);
	vTest.resize(9);
	EXPECT_FALSE(g_utilities()->Bits2Array(AA, vTest));
}

TEST_F(TestGUtilities, ConvertFromArrayToBits)
{
	const	unsigned char AA	= 0xaa; //10101010
			unsigned char test = 0x00;		
	const std::vector<unsigned char> vConst{ 1, 0, 1, 0, 1, 0, 1, 0 };
	EXPECT_TRUE(g_utilities()->Array2Bits(test, vConst));
	EXPECT_EQ(AA, test);
	const std::vector<unsigned char> vConst1{ 1, 0, 1, 0, 1, 0, 1, 0, 1 };
	EXPECT_FALSE(g_utilities()->Array2Bits(test, vConst1));
}

TEST_F(TestGUtilities, Contains)
{
    std::map<string, int>  m;
    m.emplace("entry1", 1);
    m.emplace("entry2", 2);
    m.emplace("entry3", 3);
    m.emplace("entry4", 4);

    EXPECT_TRUE(  g_utilities()->Contains(&m, (string)"entry2" ));
    EXPECT_FALSE( g_utilities()->Contains(&m, (string)"entryx" ));

}


TEST_F(TestGUtilities, isemptyNSR1804)
{
	EXPECT_FALSE( g_utilities()->IsEmpty("lorem ipsum") );
	EXPECT_TRUE(  g_utilities()->IsEmpty(" ") );
	EXPECT_TRUE(g_utilities()->IsEmpty("   ") );
	EXPECT_TRUE( g_utilities()->IsEmpty("     ") );
	EXPECT_TRUE(g_utilities()->IsEmpty("\t") );
	EXPECT_TRUE(g_utilities()->IsEmpty("\t\t") );
	EXPECT_TRUE(g_utilities()->IsEmpty("\t  \n") );
	EXPECT_TRUE(g_utilities()->IsEmpty("\n") );
	EXPECT_TRUE(g_utilities()->IsEmpty("\n\n  \t\t  ") );
}


TEST_F(TestGUtilities, containsNSR1838)
{
	vector<string> tmp_s = {"ole", "dole", "doff"};

	EXPECT_TRUE(g_utilities()->Contains(tmp_s, string("ole") ) );
	EXPECT_TRUE(g_utilities()->Contains(tmp_s, string("dole")));
	EXPECT_TRUE(g_utilities()->Contains(tmp_s, string("doff")));
	EXPECT_FALSE(g_utilities()->Contains(tmp_s, string("donald duck")));

	vector<int> tmp_i = { 1, 123, 1234 };

	EXPECT_TRUE(g_utilities()->Contains(tmp_i, 1 ) );
	EXPECT_TRUE(g_utilities()->Contains(tmp_i, 123 ) );
	EXPECT_TRUE(g_utilities()->Contains(tmp_i, 1234 ) );
	EXPECT_FALSE(g_utilities()->Contains(tmp_i, 123456) );


	vector<double> tmp_f = { 3.14, 1.68, 2.345};

	EXPECT_TRUE(  g_utilities()->Contains( tmp_f, 3.14));
	EXPECT_TRUE(  g_utilities()->Contains( tmp_f, 1.68));
	EXPECT_TRUE(  g_utilities()->Contains( tmp_f, 2.345));
	EXPECT_FALSE( g_utilities()->Contains( tmp_f, 0.618 ));

}



TEST_F(TestGUtilities, vec2stringNSR2117)
{
	string  original = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam facilisis laoreet nisi,";
	string  tab_separated = "Lorem\tipsum\tdolor\tsit\tamet,\tconsectetur\tadipiscing\telit.\tNullam\tfacilisis\tlaoreet\tnisi,";
	string  newline_separated = "Lorem\nipsum\ndolor\nsit\namet,\nconsectetur\nadipiscing\nelit.\nNullam\nfacilisis\nlaoreet\nnisi,";	
	string abc = "Loremabcipsumabcdolorabcsitabcamet,abcconsecteturabcadipiscingabcelit.abcNullamabcfacilisisabclaoreetabcnisi,";
	vector<string> splitted = {"Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit.", "Nullam", "facilisis", "laoreet", "nisi,"};

	EXPECT_EQ(original, g_utilities()->Vec2String(splitted, " ") );
	EXPECT_EQ(tab_separated, g_utilities()->Vec2String(splitted, "\t")  );
	EXPECT_EQ(newline_separated, g_utilities()->Vec2String(splitted, "\n"));
	EXPECT_EQ(abc, g_utilities()->Vec2String(splitted, "abc"));

	// check that we can conert back to the original string using the tokenizer
	vector<string> tokens = g_tokenizer()->Tokenize(tab_separated, "\t");
	string res = g_utilities()->Vec2String(tokens, " ");
	EXPECT_EQ(original, res);

	tokens = g_tokenizer()->Tokenize(newline_separated, "\n");
	res = g_utilities()->Vec2String(tokens, " ");
	EXPECT_EQ(original, res);

	tokens = g_tokenizer()->Tokenize(abc, "abc" );
	res = g_utilities()->Vec2String(tokens, " ");
	EXPECT_EQ(original, res);

}




TEST_F(TestGUtilities, filter_out)
{
	vector<int> int_vec = { 1,2, 34, 55 ,66, 66, 456, 6789 };
	vector<int> int_vec_filter = { 34, 66, 456 };

	EXPECT_EQ(8, int_vec.size() );
	EXPECT_EQ(3, int_vec_filter.size());

	for (size_t i = 0; i < int_vec.size(); i++)
	{
		EXPECT_TRUE( g_utilities()->Contains(int_vec, int_vec[i]) );
	}

	g_utilities()->FilterOut(int_vec, int_vec_filter);


	EXPECT_EQ(4, int_vec.size());
	EXPECT_EQ(3, int_vec_filter.size());

	EXPECT_TRUE( g_utilities()->Contains(int_vec, 1 ));
	EXPECT_TRUE( g_utilities()->Contains(int_vec, 2 ));
	EXPECT_FALSE( g_utilities()->Contains(int_vec, 34 ));
	EXPECT_TRUE( g_utilities()->Contains(int_vec, 55 ));
	EXPECT_FALSE( g_utilities()->Contains(int_vec, 66 ));
	EXPECT_FALSE( g_utilities()->Contains(int_vec, 456 ));
	EXPECT_TRUE( g_utilities()->Contains(int_vec, 6789 ));

	
	vector<string> s_vec			= { "sed", "do", "eiusmod", "tempor", "incididunt", "ut", "labore", "et", "dolore", "magna", "aliqua" };
	vector<string> s_vec_filter		= { "ut", "magna", "et", "et"};

	EXPECT_EQ(11, s_vec.size());
	EXPECT_EQ(4, s_vec_filter.size());

	for (size_t i = 0; i < s_vec.size(); i++)
	{
		EXPECT_TRUE(g_utilities()->Contains(s_vec, s_vec[i]));
	}

	g_utilities()->FilterOut(s_vec, s_vec_filter);

	EXPECT_EQ(8, s_vec.size());
	EXPECT_EQ(4, s_vec_filter.size());

	EXPECT_TRUE(g_utilities()->Contains(s_vec, string("sed") ) );
	EXPECT_TRUE(g_utilities()->Contains(s_vec, string("do")));
	EXPECT_TRUE(g_utilities()->Contains(s_vec, string("eiusmod")));
	EXPECT_TRUE(g_utilities()->Contains(s_vec, string("tempor")));
	EXPECT_TRUE(g_utilities()->Contains(s_vec, string("incididunt")));
	EXPECT_FALSE(g_utilities()->Contains(s_vec, string("ut")));
	EXPECT_TRUE(g_utilities()->Contains(s_vec, string("labore")));
	EXPECT_FALSE(g_utilities()->Contains(s_vec, string("et")));
	EXPECT_TRUE(g_utilities()->Contains(s_vec, string("dolore")));
	EXPECT_FALSE(g_utilities()->Contains(s_vec, string("magna")));
	EXPECT_TRUE(g_utilities()->Contains(s_vec, string("aliqua")));

}


TEST_F(TestGUtilities, ip_address_validation)
{
	EXPECT_TRUE(  g_utilities()->IsValidIPV4Address("192.168.1.7") );
	EXPECT_TRUE(  g_utilities()->IsValidIPV4Address("192.168.1.255"));
	EXPECT_TRUE(  g_utilities()->IsValidIPV4Address("10.0.11.7"));
	EXPECT_FALSE( g_utilities()->IsValidIPV4Address("10.0.11.7.2"));
	EXPECT_FALSE( g_utilities()->IsValidIPV4Address("gibberish"));
	EXPECT_FALSE( g_utilities()->IsValidIPV4Address("256.0.11.7"));
	EXPECT_TRUE(  g_utilities()->IsValidIPV4Address("255.0.11.7"));

}
