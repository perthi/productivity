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

#include "TestGRegexp.h"
#include <utilities/GRegexp.h>




TEST_F(TestGRegexp, ScanNumber )
{
    EXPECT_ANY_THROW(  g_regexp()->ScanNumber("lorem ipsum", "nonumber") );
    EXPECT_ANY_THROW(  g_regexp()->ScanNumber("lorem ipsum", "{1,2}"));
    EXPECT_NO_THROW(   g_regexp()->ScanNumber("lorem ipsum", "1,2"));
    
    vector<string> numbers;
    EXPECT_NO_THROW ( numbers =  g_regexp()->ScanNumber("At vero eos 1et accusamus 23et ius45to odio digni666ssimos ducimus qui blanditiis prae12345sentium voluptatum deleniti atque corrupti quos dolores", "1,5"));

    EXPECT_EQ( 5, numbers.size( ));

    if (numbers.size() == 5)
    {
        EXPECT_EQ(numbers.at(0), "1");
        EXPECT_EQ(numbers.at(1), "23");
        EXPECT_EQ(numbers.at(2), "45");
        EXPECT_EQ(numbers.at(3), "666");
        EXPECT_EQ(numbers.at(4), "12345");
    }

    try
    {
         g_regexp()->ScanNumber("lorem ipsum123456789");
        EXPECT_NO_THROW(numbers =  g_regexp()->ScanNumber("lorem ipsum123456789"));
        EXPECT_EQ(1, numbers.size());

        if (numbers.size() == 1)
        {
            EXPECT_EQ(numbers.at(0), "123456789");
        }
    }
    catch (std::exception &e)
    {
        cerr << e.what() << ENDL;
    }
    #ifdef HAS_LOGGING
    catch (GException &e)
    {
        cerr << e.what() << ENDL;
    }
    #endif
    catch (...)
    {
        CERR << "UNKNOWN EXCEPTION !!!!" << ENDL;
    }
}



TEST_F(TestGRegexp, is_match)
{
	string input = "Lorem ipsum dolor sit amet";
	EXPECT_TRUE( g_regexp()->IsMatch(input,  std::regex("Lorem ipsum dolor sit amet") ));
	EXPECT_TRUE( g_regexp()->IsMatch(input,  std::regex("Lorem(.*)") ));
	EXPECT_TRUE( g_regexp()->IsMatch(input,  std::regex("(.*)amet") ));
	EXPECT_TRUE( g_regexp()->IsMatch(input,  std::regex("(.*)ipsum(.*)")));
	EXPECT_TRUE( g_regexp()->IsMatch(input,  std::regex("(.*)ipsum(.*)sit(.*)") ));

	EXPECT_FALSE( g_regexp()->IsMatch(input, std::regex("blahh(.*)")));
	EXPECT_FALSE( g_regexp()->IsMatch(input, std::regex("(.*)blahh")));
	EXPECT_FALSE( g_regexp()->IsMatch(input, std::regex("(.*)blahh(.*)")));
	EXPECT_FALSE( g_regexp()->IsMatch(input, std::regex("(.*)blahh(.*)sit(.*)")));

	EXPECT_ANY_THROW(EXPECT_FALSE( g_regexp()->IsMatch(input, std::regex("*blahh*")))); // Expecst an exception since the expression is invalid (* instead of (.*))

}


TEST_F(TestGRegexp, is_match_user)
{
	string input = "Lorem ipsum dolor sit amet";
	EXPECT_TRUE( g_regexp()->IsMatch(input, "Lorem ipsum dolor sit amet"));
	EXPECT_TRUE( g_regexp()->IsMatch(input,  "Lorem*"));

	EXPECT_TRUE( g_regexp()->IsMatch(input,  "*amet" ));
	EXPECT_TRUE( g_regexp()->IsMatch(input,  "*ipsum*"));
	EXPECT_TRUE( g_regexp()->IsMatch(input,  "*ipsum*sit*" ));
	EXPECT_FALSE( g_regexp()->IsMatch(input,  "blahh*"));
	EXPECT_FALSE( g_regexp()->IsMatch(input,  "*blahh"));
	EXPECT_FALSE( g_regexp()->IsMatch(input,   "*blahh*"));
	EXPECT_FALSE( g_regexp()->IsMatch(input,  "*blahh*sit*"));

	// Testing a real case that at one time failed.
	input = "Out-Of-Band Non-Carrier related Spurious 4610.0 MHz (LOx2+2MHz) to 7000.0 MHz Spurious Level Peak [dBm]";
	EXPECT_FALSE( g_regexp()->IsMatch(input, "Lorem*"));
//	EXPECT_TRUE( g_regexp()->IsMatch(input, "*(LOx2+2MHz) to 7000.0 MHz*"));
}


TEST_F(TestGRegexp, filter)
{
	vector<string> input = { "Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit.", "Atque", "haec", "coniunctio", "confusioque", "virtutum", "tamen", "a", "philosophis", "ratione", "quadam", "distinguitur" };
	vector<string> filtered_out_expected = { "Lorem", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit.", "Atque", "haec", "coniunctio", "confusioque", "virtutum", "tamen", "a", "philosophis", "ratione", "quadam", "distinguitur" };
	vector<string> filtered_out_actual;
	vector<string> filtered =  g_regexp()->Filter(input, std::regex("ipsum"), &filtered_out_actual);
	EXPECT_EQ(vector<string>{"ipsum"}, filtered);
	EXPECT_EQ(filtered_out_expected, filtered_out_actual );
	filtered_out_actual.erase(filtered_out_actual.begin(), filtered_out_actual.end());
	filtered =  g_regexp()->Filter(input, std::regex("con(.*)"), &filtered_out_actual);
	vector<string> filtered_expected = { "consectetur", "coniunctio", "confusioque"};
	EXPECT_EQ(filtered, filtered_expected);
	filtered_out_expected = { "Lorem", "ipsum", "dolor", "sit", "amet,", "adipiscing", "elit.", "Atque", "haec", "virtutum", "tamen", "a", "philosophis", "ratione", "quadam", "distinguitur" };
	EXPECT_EQ(filtered_out_expected, filtered_out_actual);
}