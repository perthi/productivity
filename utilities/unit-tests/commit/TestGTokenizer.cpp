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


#include "TestGTokenizer.h"

#include <utilities/GTokenizer.h>
#include <utilities/GText.h>
#include <utilities/GCommon.h>
#include <utilities/GDefinitions.h>
#include <utilities/GLocation.h>

#include <ctime>


TestGTokenizer::TestGTokenizer() : TestBase()
{

}




TestGTokenizer::~TestGTokenizer()
{

}




TEST_F(TestGTokenizer, CheckEmty)
{
    string in = "a\tb\t \t\t\t";
 //   auto res = g_tokenizer()->Tokenize(in, "\t");
    auto res = GTokenizer().Tokenize(in, "\t", KEEP_EMPTY);
    EXPECT_EQ(5, res.size());
    res = GTokenizer().Tokenize(in, "\t",  DISCARD_EMPTY );
    EXPECT_EQ(2, res.size());
}



TEST_F(TestGTokenizer, Tokenize )
{
    string in1 = "a b c d";
    string in2 = "e f g h";
    string in3 = "i j k l";

    auto res = GTokenizer().Tokenize(in1, " ");
    EXPECT_EQ("a", res[0]);
    EXPECT_EQ("b", res[1]);
    EXPECT_EQ("c", res[2]);
    EXPECT_EQ("d", res[3]);

    vector<string> combo = {in1, in2, in3};
    res = GTokenizer().Tokenize(combo, " ");
   
    if (res.size() == 12)
    {
        EXPECT_EQ("a", res[0]);
        EXPECT_EQ("b", res[1]);
        EXPECT_EQ("c", res[2]);
        EXPECT_EQ("d", res[3]);

        EXPECT_EQ("e", res[4]);
        EXPECT_EQ("f", res[5]);
        EXPECT_EQ("g", res[6]);
        EXPECT_EQ("h", res[7]);

        EXPECT_EQ("i", res[8]);
        EXPECT_EQ("j", res[9]);
        EXPECT_EQ("k", res[10]);
        EXPECT_EQ("l", res[11]);
    }
    else
    {
        GTEST_FATAL_FAILURE_( GText( "Unexpected vector size %d (expected 12)", res.size() ).c_str() );
    }
   
    string d1 = "dir1\\dir2\\dir3";
    string d2 = "dir4\\dir5\\dir6";
    string d3 = "dir7\\dir8\\dir9";
    combo = {d1, d2, d3};

    res = GTokenizer().Tokenize(combo, "\\");

    if (res.size() == 9)
    {
        EXPECT_EQ("dir1", res[0]);
        EXPECT_EQ("dir2", res[1]);
        EXPECT_EQ("dir3", res[2]);
        EXPECT_EQ("dir4", res[3]);
        EXPECT_EQ("dir5", res[4]);
        EXPECT_EQ("dir6", res[5]);
        EXPECT_EQ("dir7", res[6]);
        EXPECT_EQ("dir8", res[7]);
        EXPECT_EQ("dir9", res[8]);
    }
    else
    {
        GTEST_FATAL_FAILURE_(GText("Unexpected vector size %d (expected 9)", res.size()).c_str());
    }

     string d4 = "dir1\\dir2/dir3/dir4\\dir5";
     vector<string> sep = { "\\", "/" };
     res = GTokenizer().Tokenize(d4, vector<string>{ "\\", "/" } );

    if (res.size() == 5)
    {
        EXPECT_EQ("dir1", res[0]);
        EXPECT_EQ("dir2", res[1]);
        EXPECT_EQ("dir3", res[2]);
        EXPECT_EQ("dir4", res[3]);
        EXPECT_EQ("dir5", res[4]);
    }
    else
    {
        GTEST_FATAL_FAILURE_(GText("Unexpected vector size %d (expected 5)", res.size()).c_str());
    }

    d4 = "dir1\\dir2/dir3/dir4\\dir5";
    res = GTokenizer().Tokenize(d4, vector<string>{ "\\", "/"}, DISCARD_EMPTY,  KEEP_SEPARATOR );

    if (res.size() == 5)
    {
        EXPECT_EQ("dir1\\", res[0]);
        EXPECT_EQ("dir2/", res[1]);
        EXPECT_EQ("dir3/", res[2]);
        EXPECT_EQ("dir4\\", res[3]);
        EXPECT_EQ("dir5", res[4]);
    }
    else
    {
        GTEST_FATAL_FAILURE_(GText("Unexpected vector size %d (expected 5)", res.size()).c_str());
    }
}



TEST_F(TestGTokenizer, Strip )
{
    string tmp = "dirname\\filename";
    string dir, file;
    GTokenizer().StripPath(tmp, dir, file);
    EXPECT_EQ(dir,  "dirname\\");
    EXPECT_EQ(file, "filename");

    tmp = "dirname2/filename2";
    GTokenizer().StripPath(tmp, dir, file);
    EXPECT_EQ(dir, "dirname2/");
    EXPECT_EQ(file, "filename2");

    tmp = "dirname3/filename3";
    GTokenizer().StripPath(tmp, dir, file, DISCAR_TRAILING_SEPARATOR);
    EXPECT_EQ(dir,  "dirname3");
    EXPECT_EQ(file, "filename3");

    tmp = "dir2/dir3/dir4/file.txt";
    GTokenizer().StripPath(tmp, dir, file);
    EXPECT_EQ(dir, "dir2/dir3/dir4/");
    EXPECT_EQ(file, "file.txt");
 }



TEST_F(TestGTokenizer, GTokenizerNSR246)
{
    try
    {
        string lpath = "/dir1/dir2/dir3/file.txt";
        string wpath = "\\dir1\\dir2\\dir2\\file.txt";
        vector<string> ltokens = GTokenizer().Tokenize(lpath, "/");
        vector<string> wtokens = GTokenizer().Tokenize(wpath, "\\");
        EXPECT_EQ(4, ltokens.size());
        EXPECT_EQ("dir1", ltokens[0]);
        EXPECT_EQ("dir2", ltokens[1]);
        EXPECT_EQ("dir3", ltokens[2]);
        EXPECT_EQ("file.txt", ltokens[3]);
        EXPECT_EQ(4, wtokens.size());
        EXPECT_EQ("dir1", wtokens.at(0));
        EXPECT_EQ("dir2", wtokens.at(1));
        EXPECT_EQ("file.txt", wtokens.at(3));
    }
    catch (std::exception &e)
    {
        GCommon().HandleError( GText( "STD Exception caught:\t %s", e.what() ).str(),GLOCATION, DISABLE_EXCEPTION  );
    }
    #ifdef HAS_LOGGING
    catch (GException &e)
    {

        G_ERROR("STD Exception caught:\t %s", e.what());
    }
    #endif

    catch (...)
    {
        GCommon().HandleError(  "Unknown Exception caught" ,GLOCATION, DISABLE_EXCEPTION  );
    }
}


TEST_F(TestGTokenizer, NSR1133)
{
    string path = "myfile.cpp";
    string dir, filename;
    GTokenizer().StripPath(path, dir, filename);

    EXPECT_EQ(path, filename);

    path = "mydir/";
    GTokenizer().StripPath(path, dir, filename);
    EXPECT_EQ(path, dir);
    EXPECT_EQ("", filename );

    path = "mydir\\";
    GTokenizer().StripPath(path, dir, filename);
    EXPECT_EQ(path, dir);
    EXPECT_EQ("", filename);

    path = "dir\\dir2\\mydir\\";
    GTokenizer().StripPath(path, dir, filename);
    EXPECT_EQ(path, dir);
    EXPECT_EQ("", filename);

}


TEST_F(TestGTokenizer, bugNSR2152)
{
    string input = "Loremabcipsumabcdolorabcsitabcamet,abcconsecteturabcadipiscingabcelit.abcNullamabcfacilisisabclaoreetabcnisi,";  // Lorem ipsum separated by ABC
    vector<string> expected_tokens = { "Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit.", "Nullam", "facilisis", "laoreet", "nisi," };
    vector<string> actual_tokens =  GTokenizer().Tokenize(input, "abc");
    EXPECT_EQ(expected_tokens, actual_tokens);
}


TEST_F(TestGTokenizer, tokenize_commandline)
{
    string cmdline = "./application -arg1 -arg2 -arg3";
    vector<string> tokens = GTokenizer().TokenizeCommandline(cmdline);
    ASSERT_EQ(  tokens.size(), 4 );
    EXPECT_EQ(  tokens.at(0),  "./application");
    EXPECT_EQ(  tokens.at(1),  "-arg1");
    EXPECT_EQ(  tokens.at(2),  "-arg2");
    EXPECT_EQ(  tokens.at(3),  "-arg3");
}


TEST_F(TestGTokenizer, tokenize_commandline2)
{
    //string cmdline = "./application -arg1 -arg2 -arg2 -arg4 -arg5";
    string cmdline = "-arg1 -arg2 -arg3 -arg4 -arg5";
    const char *argv[10];
    int argc;
    GTokenizer().TokenizeCommandline( cmdline, &argc, argv, 10 );
    ASSERT_EQ( argc, 6 );

    EXPECT_STREQ( argv[1], "-arg1" );
    EXPECT_STREQ( argv[2], "-arg2" );
    EXPECT_STREQ( argv[3], "-arg3" );
    EXPECT_STREQ( argv[4], "-arg4" );
    EXPECT_STREQ( argv[5], "-arg5" );
}


#ifdef NDEBUG
TEST_F(TestGTokenizer, Strip_performance )
{
#ifdef HAS_LOGGING
    SET_LOGTARGET("--target-file");
    SET_LOGLEVEL("--error");
#endif

    int n = 100000;

    string tmp1 = "dirname\\filename";
    string tmp2 = "dirname2/filename2";
    string tmp3 = "dirname3/filename3";
    string tmp4 = "dir2/dir3/dir4/file.txt";
    string dir, file;    

    clock_t begin = clock();    
    for ( int i = 0; i < n; i++ )
    {
        GTokenizer().StripPath( tmp1, dir, file );
        GTokenizer().StripPath( tmp2, dir, file );
        GTokenizer().StripPath( tmp3, dir, file, DISCAR_TRAILING_SEPARATOR );
        GTokenizer().StripPath( tmp4, dir, file );
    }

    clock_t end = clock();
    double elapsed_secs = double( (double)end - (double)begin );
    double average = 1000*(elapsed_secs / n);
    //ASSERT_TRUE( average < 0.05 );
    printf("Average time for strip is %4.8f milliseconds (elapsed sec = %f)\n", average, elapsed_secs );
 }
#endif
