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

#include "TestGString.h"
#include <utilities/GString.h>
#include <utilities/GTokenizer.h>
#include <utilities/GConstants.h>

using namespace GCONSTANTS;



TEST_F(TestGString, ReplaceBadChar)
{
    char tmp[] = "&&this%#&!!isateststring";
     g_string()->ReplaceBadChar(tmp, '_');
   EXPECT_STREQ(tmp, "__this_____isateststring" );

   string s = "another&&&stri%%[]{}ng";
    g_string()->ReplaceBadChar(s, '-');
   EXPECT_STREQ(s.c_str(), "another---stri------ng");

   string s2 = "yetanother&&&#??stri%%[]{}ng";
    g_string()->ReplaceBadChar(s2, '-', "&#");
   EXPECT_STREQ(s2.c_str(), "yetanother----??stri%%[]{}ng");
}


TEST_F(TestGString, Replace)
{
    string original = "for every complicated problem ther is simple, elegant and wrong answer";
    string modified =  g_string()->Replace(original, "complicated", "complex" );
    EXPECT_EQ("for every complex problem ther is simple, elegant and wrong answer", modified);
    modified =  g_string()->Replace(modified, "wrong", "incorrect");
    EXPECT_EQ("for every complex problem ther is simple, elegant and incorrect answer", modified);
}


TEST_F(TestGString, Replace2)
{
	string original = "*ipsum*";
	string modified =  g_string()->Replace(original, "*",  "(.*)" );

	EXPECT_EQ("(.*)ipsum(.*)", modified);
}

TEST_F(TestGString, Trim)
{
    string tmp = "abcd     ";
    EXPECT_EQ("abcd",    g_string()->Rtrim(tmp));
    tmp = "   efgh ";
    EXPECT_EQ("efgh",    g_string()->Trim( tmp ));
    tmp =  "       i j k l m n o p       ";
    EXPECT_EQ("i j k l m n o p", g_string()->Trim(tmp));
    tmp =  "   ���  ";
    EXPECT_EQ("   ���",  g_string()->Rtrim(tmp));
    tmp = "   ���  ";
    EXPECT_EQ("���  ",   g_string()->Ltrim(tmp));
    tmp = "   ���  ";
    EXPECT_EQ("���",     g_string()->Trim(tmp) );
    tmp =  "� ";
    EXPECT_EQ("�",       g_string()->Rtrim(tmp) );
    tmp = "-0x7FFFFFFFFFFFFFFF";
    const char token = '-'; 
	EXPECT_EQ("0x7FFFFFFFFFFFFFFF", g_string()->Trim(tmp, '-' ));
    tmp = "-0xadef123"; 
    EXPECT_EQ("0xadef123", g_string()->Trim( tmp, token ));
}



TEST_F(TestGString, TrimVector)
{
    string test = "a,b,c, d, e   ,f  , g,h  , i ";   
    vector<string>  tokens = g_tokenizer()->Tokenize(test, ",");

    EXPECT_EQ(tokens.size(), 9);

    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
    EXPECT_EQ(tokens[2], "c");
    EXPECT_EQ(tokens[3], " d");
    EXPECT_EQ(tokens[4], " e   ");
    EXPECT_EQ(tokens[5], "f  ");
    EXPECT_EQ(tokens[6], " g");
    EXPECT_EQ(tokens[7], "h  ");
    EXPECT_EQ(tokens[8], " i ");

     g_string()->Trim(tokens);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
    EXPECT_EQ(tokens[2], "c");
    EXPECT_EQ(tokens[3], "d");
    EXPECT_EQ(tokens[4], "e");
    EXPECT_EQ(tokens[5], "f");
    EXPECT_EQ(tokens[6], "g");
    EXPECT_EQ(tokens[7], "h");
    EXPECT_EQ(tokens[8], "i");

}


TEST_F(TestGString, TrimVector_multiple_tokens)
{
    string test = "a,b,c, \td, e\n\n   , \tf  , g,h  \n,   \t i    \n \t ";

    vector<string>  tokens = g_tokenizer()->Tokenize(test, ",");


    EXPECT_EQ(tokens.size(), 9);

    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
    EXPECT_EQ(tokens[2], "c");
    EXPECT_EQ(tokens[3], " \td");
    EXPECT_EQ(tokens[4], " e\n\n   ");
    EXPECT_EQ(tokens[5], " \tf  ");
    EXPECT_EQ(tokens[6], " g");
    EXPECT_EQ(tokens[7], "h  \n");
    EXPECT_EQ(tokens[8], "   \t i    \n \t ");

     g_string()->Trim(tokens, {' ', '\t', '\n'});
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
    EXPECT_EQ(tokens[2], "c");
    EXPECT_EQ(tokens[3], "d");
    EXPECT_EQ(tokens[4], "e");
    EXPECT_EQ(tokens[5], "f");
    EXPECT_EQ(tokens[6], "g");
    EXPECT_EQ(tokens[7], "h");
    EXPECT_EQ(tokens[8], "i");

}



//using namespace std::string_literals;
TEST_F(TestGString, substringReplacementNSR387)
{
    string in = "lorem ipsum";
    string modified =  g_string()->Replace(in, "rem", "REM");
    EXPECT_EQ("loREM ipsum",  modified);
     modified =  g_string()->Replace(in, "ipsum", "ipsum dolor sit");
    EXPECT_EQ("lorem ipsum dolor sit", modified);
    modified =  g_string()->Replace(in, "lorem ipsum", "lorem ipsum");
    EXPECT_EQ("lorem ipsum", modified);
    modified =  g_string()->Replace(in, "X", "Y");
    EXPECT_EQ("lorem ipsum", modified);
    in = "//ames-linux01/svn/knts";
    modified =  g_string()->Replace(in, "/", "\\");
    EXPECT_EQ("\\\\ames-linux01\\svn\\knts",  modified );
    in = "Hello Dolly";
    modified =  g_string()->Replace(in, "y", "Y");
    EXPECT_EQ("Hello DollY", modified);
    in = R"(Path: C:\work\knts)";
    modified =  g_string()->Replace(in, "", "");
    EXPECT_EQ("Path: C:\\work\\knts", modified);
}




TEST_F(TestGString, ToString)
{
    EXPECT_EQ("9223372036854775807",  g_string()->ToString(9223372036854775807));
    EXPECT_EQ("123",  g_string()->ToString(123));
    EXPECT_EQ("000123",  g_string()->ToString(123, 6 ));
    EXPECT_EQ("12345678",  g_string()->ToString(12345678, 4));
	vector<int> tmp{ 1,2,3,4,5,6 };
	EXPECT_EQ("123456",  g_string()->ToString(tmp, string("")));
	EXPECT_EQ("1\n2\n3\n4\n5\n6",  g_string()->ToString(tmp));
    vector<double> d{ 1.2,2.3777 ,3.4};
    EXPECT_EQ("1.2\n2.3777\n3.4",  g_string()->ToString(d));
    vector<string> s = {"ole", "dole", "doff"};
    EXPECT_EQ("ole dole doff",  g_string()->ToString(s, string(" ") ));
    vector<char> c = { 'a', 'b', 'c', 'd', 'e',  'f',  'g',  'h' };
    EXPECT_EQ("abcdefgh",  g_string()->ToString(c, string("")));
}




TEST_F(TestGString, Contains)
{
    string tmp = "blahhhloremipsum_blahhhhhlorem_xxxx";
    string substr = "Lorem";
    long long pos = -1;
    bool contains =  g_string()->Contains(tmp, substr, true, &pos);
    EXPECT_EQ(true, contains);
    EXPECT_EQ(6, pos);
    pos = -1;
    contains =  g_string()->Contains(tmp, substr, false, &pos);
    EXPECT_EQ(false, contains);
    EXPECT_EQ(std::string::npos, pos);
    contains =  g_string()->Contains(tmp, "testxxx", true, &pos);
    EXPECT_EQ(false, contains);
    EXPECT_EQ(std::string::npos, pos);
    EXPECT_TRUE( g_string()->Contains("Chuck Norris", "Chuck"));
    EXPECT_FALSE( g_string()->Contains("", "Chuck"));
    EXPECT_FALSE( g_string()->Contains("", "any string"));
}



TEST_F(TestGString, CountOccurences)
{
    const string lorem = "At vero eos et accusamus et iusto odio dignissimos ducimus qui blanditiis praesentium voluptatum deleniti atque corrupti quos dolores";
     EXPECT_EQ(2,   g_string()->CountOccurences(lorem, 'V',fgkIGNORE_CASE));
     EXPECT_EQ(0,  g_string()->CountOccurences(lorem, 'V', fgkCASE_SENSITIVE ));
     EXPECT_EQ(10,  g_string()->CountOccurences(lorem, 'e', fgkIGNORE_CASE));
     EXPECT_EQ(10,  g_string()->CountOccurences(lorem, 'E', fgkIGNORE_CASE));
     
     EXPECT_EQ(0,  g_string()->CountOccurences(lorem, 'E', fgkCASE_SENSITIVE));
     EXPECT_EQ(14,  g_string()->CountOccurences(lorem, 'i', fgkIGNORE_CASE));

     EXPECT_EQ(1,  g_string()->CountOccurences(lorem, 'A', fgkCASE_SENSITIVE));
     EXPECT_EQ(7,  g_string()->CountOccurences(lorem, 'A', fgkIGNORE_CASE));
    
     EXPECT_EQ(6,  g_string()->CountOccurences(lorem, 'a', fgkCASE_SENSITIVE));
     EXPECT_EQ(7,  g_string()->CountOccurences(lorem, 'a', fgkIGNORE_CASE));
}


#ifdef _WIN32
TEST_F(TestGString, ToStringBUG_NSR1049)
{
    EXPECT_EQ( "1300.12312344",  g_string()->ToString(1300.12312344));
    EXPECT_EQ("1300.1231234500001",  g_string()->ToString(1300.12312345)); 
    EXPECT_EQ("1300.12345678",  g_string()->ToString<long double>(1300.12345678));
    EXPECT_EQ("13000.12345678",  g_string()->ToString<long double>(13000.12345678));
	EXPECT_EQ(  "130000.1234567123",  g_string()->ToString<long double>((long double)130000.1234567123));
}
#endif


TEST_F(TestGString, Utf8ToAnsi)
{
	string s = "ABC \xC2\xA2 123";

	EXPECT_EQ("ABC \xA2 123",  g_string()->Utf8ToAnsi(s));
	EXPECT_EQ("ABC \xA2 123", s);
}



//#ifdef _WIN32
//TEST_F(TestGString, AnsiToUtf8)
//{
//	const std::string makeNoChanges = "A sentence without special characters";
//	const std::string norwegianAnsi = "� � � � � �";
//	const std::string norwegianUtf8 = "\xC3\xA6 \xC3\xb8 \xC3\xA5 \xC3\x86 \xC3\x98 \xC3\x85";
//
//	EXPECT_EQ(makeNoChanges,  g_string()->AnsiToUtf8(makeNoChanges));
//	EXPECT_EQ(norwegianUtf8,  g_string()->AnsiToUtf8(norwegianAnsi));
//}
//#endif


#ifdef _WIN32
TEST_F(TestGString, ToLower)
{
	string s = "ABC \xC2\xA2 123";
	EXPECT_EQ("abc \xC2\xA2 123",  g_string()->ToLower(s));
	EXPECT_EQ("ABC \xC2\xA2 123", s);
	s = "ABC \xA2 123";
	EXPECT_EQ("abc \xA2 123",  g_string()->ToLower(s));
	s = "ABC \xC5 123";
	EXPECT_EQ("abc \xE5 123",  g_string()->ToLower(s));
	s = "ABC \xC3\x85 123";
	EXPECT_EQ("abc \xC3\xA5 123",  g_string()->ToLower(s));
}
#endif


#ifdef _WIN32
TEST_F(TestGString, ToUpper)
{
	string s = "abc \xC2\xA2 123";

	EXPECT_EQ("ABC \xC2\xA2 123",  g_string()->ToUpper(s));
	EXPECT_EQ("abc \xC2\xA2 123", s);

	s = "abc \xA2 123";
	EXPECT_EQ("ABC \xA2 123",  g_string()->ToUpper(s));

	s = "abc \xE5 123";
	EXPECT_EQ("ABC \xC5 123",  g_string()->ToUpper(s));

	s = "abc \xC3\xA5 123";
	EXPECT_EQ("ABC \xC3\x85 123",  g_string()->ToUpper(s));
}
#endif


TEST_F(TestGString, is_match)
{
	string pattern = "123";
	string val = "123456";

	EXPECT_TRUE(  g_string()->IsMatch(pattern, val) );
	EXPECT_FALSE(  g_string()->IsMatch(pattern, val, true ));

}


TEST_F(TestGString, beginswidth_empty)
{
	string s = "lorem ipsum";
	EXPECT_TRUE( g_string()->BeginsWith( s, "" ));
}
