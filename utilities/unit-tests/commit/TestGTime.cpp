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

#include "TestGTime.h"
#include <utilities/GTime.h>
#include <utilities/GTimeFormat.h>
#include <utilities/GTimeValidate.h>
#include <utilities/GRandom.h>
#include <utilities/GRegexp.h>



TestGTime::TestGTime() : TestBase(), fTime(), fTimeValidate()
{

}




TEST_F(TestGTime, NSR_242)
{
    int iLeapYears[146] = { 1804, 1808, 1812, 1816, 1820, 1824, 1828, 1832, 1836, 1840,    1844, 1848,    1852, 1856,    1860, 1864,    1868, 1872,
        1876, 1880, 1884, 1888, 1892, 1896, 1904, 1908, 1912, 1916, 1920, 1924, 1928, 1932, 1936, 1940,    1944, 1948,
        1952, 1956, 1960, 1964, 1968, 1972, 1976, 1980, 1984, 1988, 1992, 1996, 2000, 2004, 2008, 2012, 2016, 2020,
        2024, 2028, 2032, 2036, 2040, 2044, 2048, 2052, 2056, 2060, 2064, 2068, 2072, 2076, 2080, 2084, 2088, 2092,
        2096, 2104, 2108, 2112, 2116, 2120,    2124, 2128, 2132, 2136, 2140, 2144, 2148, 2152, 2156, 2160, 2164, 2168,
        2172, 2176, 2180, 2184, 2188, 2192,    2196, 2204, 2208, 2212, 2216, 2220, 2224, 2228, 2232, 2236, 2240, 2244,
        2248, 2252, 2256, 2260, 2264, 2268, 2272, 2276, 2280, 2284, 2288, 2292, 2296, 2304, 2308, 2312, 2316, 2320,
        2324, 2328, 2332, 2336, 2340, 2344, 2348, 2352, 2356, 2360, 2364, 2368, 2372, 2376, 2380, 2384, 2388, 2392,
        2396, 2400 };

    for (int i = 0; i < 146; i++)
    {
        EXPECT_EQ(true, fTimeValidate.IsGregorianLeapYear(iLeapYears[i]));
    }

    for (int n = 1800; n < 2401; n++)
    {
        bool isLeap = false;

        for (int i = 0; i < 146; i++)
        {
            if (iLeapYears[i] == n)
            {
                isLeap = true;
            }
        }
        if (isLeap == false)
        {
            EXPECT_EQ(false, fTimeValidate.IsGregorianLeapYear(n));
        }
    }
}




TEST_F(TestGTime, TimeStamp)
{
    string offender;


    try
    {
        EXPECT_TRUE( GTimeValidate().IsValidDayS( GTime().TimeStamp(0, "%a")));
        EXPECT_TRUE( GTimeValidate().IsValidDayS( GTime().TimeStamp(0, "%A")));
        EXPECT_TRUE( GTimeValidate().IsValidDayS( GTime().TimeStamp(0, "%a")));
        EXPECT_TRUE( GTimeValidate().IsValidMonthS( GTime().TimeStamp(0, "%b")));
        EXPECT_TRUE( GTimeValidate().IsValidMonthS( GTime().TimeStamp(0, "%B")));
        EXPECT_TRUE( GTimeValidate().IsValidMonthS( GTime().TimeStamp(0, "%m")));
        EXPECT_TRUE( GTimeValidate().IsValidDateS( GTime().TimeStamp(0, "%e")));
        EXPECT_TRUE( GTimeValidate().IsValidDateS( GTime().TimeStamp(0, "%d")));
        EXPECT_TRUE( GTimeValidate().IsValidHourS( GTime().TimeStamp(0, "%H")));
        EXPECT_TRUE( GTimeValidate().IsValidFormat("%a%b", offender));
        EXPECT_FALSE( GTimeValidate().IsValidFormat("%a%l", offender));
        EXPECT_EQ("l", offender);
        EXPECT_NO_THROW( GTime().TimeStamp(0, "%a%B%m"));
        // We create an arbritary format string, but with valid tokens, just to check that our TimeStamp algorithm works generically 

        auto   format_tokens =  GTimeFormat().GetFormatSpecifiersV();
        
        size_t n = format_tokens.size();
        size_t i = g_random()->Uniform<size_t>(0, n - 1);
            
        string s1 = format_tokens.at(i);
        i = g_random()->Uniform<size_t>(0, n - 1);
        string s2 = format_tokens.at(i);
        i = g_random()->Uniform<size_t>(0, n - 1);
        string s3 = format_tokens.at(i);
        string formatstring = s1 + "miney" + s2 + "moe" + s3 + "meeny";
        
         fTime.TimeStamp(0, formatstring.c_str());
       
        //  For values less than 10 the date is padded with either zeroes or blanks for the %e and %d format tag respectively, we need to verify 
        //  that this works too by "faking" a random date between zero and 9  
        struct std::tm dt;
        int day = g_random()->Uniform<int>(1, 9);
        dt.tm_mday = day;
        EXPECT_TRUE( fTimeValidate.IsValidDateS( fTime.TimeStamp(0, "%e", &dt)));
        EXPECT_TRUE( fTimeValidate.IsValidDateS( fTime.TimeStamp(0, "%d", &dt)));
        EXPECT_ANY_THROW( fTime.TimeStamp(0, "%a%b%J%H"));
    }
#ifdef HAS_LOGGING
    catch (GException& e)
    {
        CERR << e.what() << ENDL;
    }
#endif
    catch (std::exception& e)
    {
        CERR << e.what() << ENDL;
    }
    catch (...)
    {
        CERR << "Unknown exception caught" << ENDL;
    }
    
}



TEST_F(TestGTime, ValidDayTest)
{
    vector<string> valid = { "1", "2", "3", "4", "5", "6", "7","Monday", "monday", "Mon", "Tuesday", "tuesday", "Tue", "tues", "Wednesday", "Wed", "Thursday", "Thu", "Friday", "Fri", "Saturday", "Sat", "Sunday", "Sun" };
    vector<string> invalid = { "-1", "8", "99", "blahh", "blop", "41", "9999",  "Meeny, Miny", "Moe", "12", "blahh", "January" };

    for (auto d : valid )
    {
        EXPECT_TRUE( fTimeValidate.IsValidDayS(d) );
    }

     for (auto d : invalid )
    {
        EXPECT_FALSE( fTimeValidate.IsValidDayS(d) );
    }

}



TEST_F(TestGTime, ValidDateTest)
{
    vector<string> valid { "1" ,"2","3","4" ,"5" ,"6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31" };
    vector<string> invalid{ "-101", "33", "666", "42", "blahhhh", "blop", "monday", "tuesday" };
    
    for (auto d : valid )
    {
        EXPECT_TRUE(  fTimeValidate.IsValidDateS(d) );
    }

    for (auto d : invalid )
    {
        EXPECT_FALSE(  fTimeValidate.IsValidDateS(d) );
    }

//    Validate( GTime().IsValidDateS,  GTime().IsValidDateS, DATE_MIN, DATE_MAX, valid_dates, invalid_dates);
}



TEST_F(TestGTime, ValidMonth)
{
    vector <string> valid = { "January", "jan", "Jan", "February","Feb", "feb", "March", "Ma", "Mar", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
    vector<string> invalid{ "-101", "33", "666", "42", "blahhhh", "blop", "monday", "tuesday" };
    
      
    for (auto d : valid )
    {
        EXPECT_TRUE(  fTimeValidate.IsValidMonthS(d) );
    }

    for (auto d : invalid )
    {
        EXPECT_FALSE(  fTimeValidate.IsValidMonthS(d) );
    }
    
   // Validate( GTime().IsValidMonthS,  GTime().IsValidMonthS, MONTH_MIN, MONTH_MAX, valid_months, invalid_months);
}


TEST_F(TestGTime, ValidYear)
{
    vector<string> valid = { "2019", "2030", "2020", "2053" };
    vector<string> invalid { "-1", "Meeny, Miny", "Moe", "100000", "Monday", "january", "-42" };
    for (auto d : valid )
    {
        EXPECT_TRUE(  fTimeValidate.IsValidYearS(d) );
    }

    for (auto d : invalid )
    {
        EXPECT_FALSE(  fTimeValidate.IsValidYearS(d) );
    }

   // Validate( GTime().IsValidYearS,  GTime().IsValidYearS, YEAR_MIN, YEAR_MAX, valid_years, invalid_years);
}


TEST_F(TestGTime, ValidHour)
{
    vector<string> valid = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "19", "22", "23" };
    vector<string> invalid { "-1", "Meeny, Miny", "Moe", "100000", "Monday", "january", "-42" };

    for (auto d : valid )
    {
        EXPECT_TRUE( fTimeValidate.IsValidHourS(d) );
    }

    for (auto d : invalid )
    {
        EXPECT_FALSE(  fTimeValidate.IsValidHourS(d) );
    }
    
//    Validate( GTime().IsValidHourS,  GTime().IsValidHourS, HOUR_MIN, HOUR_MAX, valid_hrs, invalid_hrs);
}


TEST_F(TestGTime, ValidMinutes)
{
    vector<string> valid = { "0", "11", "15", "23", "32", "45", "56", "57", "60", "3" };
    vector<string> invalid{ "-1", "120, Miny", "Moe", "100000", "Monday", "blahh", "-45" };
    
    for (auto d : valid )
    {
        EXPECT_TRUE(  fTimeValidate.IsValidMinuteS(d) );
    }

    for (auto d : invalid )
    {
        EXPECT_FALSE(  fTimeValidate.IsValidMinuteS(d) );
    }

    //Validate( GTime().IsValidMinuteS,  GTime().IsValidMinuteS, MINUTE_MIN, MINUTE_MAX, valid_mins, invalid_mins);
}


TEST_F(TestGTime, ValidSeconds)
{
    vector<string> valid = { "0", "11", "15", "23", "32", "45", "56", "57", "60", "3" };
    vector<string> invalid{ "-1", "120, Miny", "Moe", "100000", "Monday", "blahh", "-45" };
    
    for (auto d : valid )
    {
        EXPECT_TRUE(  fTimeValidate.IsValidSecondS(d) );
    }

    for (auto d : invalid )
    {
        EXPECT_FALSE(  fTimeValidate.IsValidSecondS(d) );
    }

   // Validate( GTime().IsValidSecondS,  GTime().IsValidSecondS, SECONDS_MIN, SECONDS_MAX, valid_s, invalid_s);
}



TEST_F(TestGTime, NSR279FormatString)
{
    string date;
    EXPECT_NO_THROW(   fTime.TimeStamp("%H"));
    EXPECT_ANY_THROW( fTime.TimeStamp("%K"));
    EXPECT_NO_THROW(date =  fTime.TimeStamp("%H:%M:%S:%m:%Y"));
    vector<string> tokens = g_regexp()->ScanNumber(date);
    EXPECT_EQ(5, tokens.size());

    if (tokens.size() == 5)
    {
        EXPECT_TRUE( fTimeValidate.IsValidHour(g_numbers()->ToInteger<int>(tokens[0])));
        EXPECT_TRUE( fTimeValidate.IsValidMinute(g_numbers()->ToInteger<int>(tokens[1])));
        EXPECT_TRUE( fTimeValidate.IsValidSecond(g_numbers()->ToInteger<int>(tokens[2])));
        EXPECT_TRUE( fTimeValidate.IsValidMonth(g_numbers()->ToInteger<int>(tokens[3])));
        EXPECT_TRUE( fTimeValidate.IsValidYear(g_numbers()->ToInteger<int>(tokens[4])));
    }
}





TEST_F(TestGTime, verifyAccessDateNSR275)
{
    //    EXPECT_NEAR(Time::AccessDate("2016.01.26 12:00:00"),42395.50, 0.001, 001);
    //    EXPECT_DOUBLE_EQ(Time::AccessDate("2016.07.20 10:15:30"), 42571.427430555600, 001); // 42571.427430555554, 001);

    EXPECT_NEAR(fTime.AccessDate("2016.07.20 10:15:30"), 42571.427430555600, 0.001);
    EXPECT_NEAR((float)fTime.AccessDate("2017.07.20 10:15:30"), 42936.430, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2018.07.20 10:15:30"), 43301.430, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2019.07.20 10:15:30"), 43666.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2020.07.20 10:15:30"), 44032.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2021.07.20 10:15:30"), 44397.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2022.07.20 10:15:30"), 44762.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2023.07.20 10:15:30"), 45127.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2024.07.20 10:15:30"), 45493.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2025.07.20 10:15:30"), 45858.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2026.07.20 10:15:30"), 46223.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2027.07.20 10:15:30"), 46588.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2028.07.20 10:15:30"), 46954.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2029.07.20 10:15:30"), 47319.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2030.07.20 10:15:30"), 47684.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2031.07.20 10:15:30"), 48049.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2032.07.20 10:15:30"), 48415.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2033.07.20 10:15:30"), 48780.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2034.07.20 10:15:30"), 49145.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2035.07.20 10:15:30"), 49510.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2036.07.20 10:15:30"), 49876.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2037.07.20 10:15:30"), 50241.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2038.07.20 10:15:30"), 50606.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2039.07.20 10:15:30"), 50971.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2040.07.20 10:15:30"), 51337.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2041.07.20 10:15:30"), 51702.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2042.07.20 10:15:30"), 52067.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2043.07.20 10:15:30"), 52432.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2044.07.20 10:15:30"), 52798.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2045.07.20 10:15:30"), 53163.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2046.07.20 10:15:30"), 53528.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2047.07.20 10:15:30"), 53893.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2048.07.20 10:15:30"), 54259.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2049.07.20 10:15:30"), 54624.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2050.07.20 10:15:30"), 54989.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2051.07.20 10:15:30"), 55354.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2052.07.20 10:15:30"), 55720.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2053.07.20 10:15:30"), 56085.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2054.07.20 10:15:30"), 56450.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2055.07.20 10:15:30"), 56815.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2056.07.20 10:15:30"), 57181.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2057.07.20 10:15:30"), 57546.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2058.07.20 10:15:30"), 57911.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2059.07.20 10:15:30"), 58276.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2060.07.20 10:15:30"), 58642.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2061.07.20 10:15:30"), 59007.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2062.07.20 10:15:30"), 59372.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2063.07.20 10:15:30"), 59737.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2064.07.20 10:15:30"), 60103.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2065.07.20 10:15:30"), 60468.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2066.07.20 10:15:30"), 60833.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2067.07.20 10:15:30"), 61198.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2068.07.20 10:15:30"), 61564.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2069.07.20 10:15:30"), 61929.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2070.07.20 10:15:30"), 62294.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2071.07.20 10:15:30"), 62659.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2072.07.20 10:15:30"), 63025.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2073.07.20 10:15:30"), 63390.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2074.07.20 10:15:30"), 63755.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2075.07.20 10:15:30"), 64120.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2076.07.20 10:15:30"), 64486.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2077.07.20 10:15:30"), 64851.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2078.07.20 10:15:30"), 65216.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2079.07.20 10:15:30"), 65581.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2080.07.20 10:15:30"), 65947.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2081.07.20 10:15:30"), 66312.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2082.07.20 10:15:30"), 66677.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2083.07.20 10:15:30"), 67042.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2084.07.20 10:15:30"), 67408.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2085.07.20 10:15:30"), 67773.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2086.07.20 10:15:30"), 68138.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2087.07.20 10:15:30"), 68503.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2088.07.20 10:15:30"), 68869.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2089.07.20 10:15:30"), 69234.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2090.07.20 10:15:30"), 69599.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2091.07.20 10:15:30"), 69964.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2092.07.20 10:15:30"), 70330.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2093.07.20 10:15:30"), 70695.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2094.07.20 10:15:30"), 71060.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2095.07.20 10:15:30"), 71425.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2096.07.20 10:15:30"), 71791.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2097.07.20 10:15:30"), 72156.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2098.07.20 10:15:30"), 72521.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2099.07.20 10:15:30"), 72886.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2100.07.20 10:15:30"), 73251.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2101.07.20 10:15:30"), 73616.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2102.07.20 10:15:30"), 73981.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2103.07.20 10:15:30"), 74346.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2104.07.20 10:15:30"), 74712.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2105.07.20 10:15:30"), 75077.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2106.07.20 10:15:30"), 75442.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2107.07.20 10:15:30"), 75807.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2108.07.20 10:15:30"), 76173.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2109.07.20 10:15:30"), 76538.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2110.07.20 10:15:30"), 76903.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2111.07.20 10:15:30"), 77268.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2112.07.20 10:15:30"), 77634.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2113.07.20 10:15:30"), 77999.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2114.07.20 10:15:30"), 78364.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2115.07.20 10:15:30"), 78729.43, 001);
    EXPECT_NEAR((float)fTime.AccessDate("2116.07.20 10:15:30"), 79095.43, 001);
}



#ifdef _WIN32
TEST_F(TestGTime, stringToDateConversionNSR1287)
{
    std::tm t;
    GTime().DateString2Time("2017-February-18 23:12:34",  "%Y-%b-%d %H:%M:%S", &t);
    
    EXPECT_EQ(t.tm_hour, 23);  
    EXPECT_EQ(t.tm_mday, 18);  
    EXPECT_EQ(t.tm_min, 12 );   
    EXPECT_EQ(t.tm_sec, 34  );  
    EXPECT_EQ(t.tm_mon, 1);    // NB counting starts at ZERO so February = month 1 
    EXPECT_EQ(t.tm_year, 117 ); // Number of years since 1/1 1900+
    
    GTime().DateString2Time("2009-June-23 10:09:55", "%Y-%b-%d %H:%M:%S", &t);

    EXPECT_EQ(t.tm_hour, 10);   
    EXPECT_EQ(t.tm_mday, 23);   
    EXPECT_EQ(t.tm_min, 9);     
    EXPECT_EQ(t.tm_sec, 55);    
    EXPECT_EQ(t.tm_mon, 5);     // NB counting starts at ZERO so February = month 1 
    EXPECT_EQ(t.tm_year, 109);  // Number of years since 1/1 1900+
   
    int64_t us;

    GTime().DateString2Time("2009-June-23 10:09:49.123456", "%Y-%b-%d %H:%M:%S", &t, &us);
    EXPECT_EQ(t.tm_sec, 49);
    EXPECT_EQ(123456, us);
    GTime().DateString2Time("2014-April-23 10:09:49.000456", "%Y-%b-%d %H:%M:%S", &t, &us);
    EXPECT_EQ(456, us);
    GTime().DateString2Time("2002-April-16 10:09:49.456000", "%Y-%b-%d %H:%M:%S", &t, &us);
    EXPECT_EQ(456000, us);
}
#endif



