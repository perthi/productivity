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

#include "TestGFileIOHandler.h"
#include <utilities/GFileIOHandler.h>
#include <utilities/GRandom.h>
#include <utilities/GSystem.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#undef CreateFile


void 
TestGFileIOHandler::SetUpTestCase()
{
    GCommon().DisableOutput();
}


void 
TestGFileIOHandler::TearDownTestCase()
{
    GCommon().EnableOutput();
}
 

/**
* Here we check that the Utilities::CheckFile  function actually does what is expected, and that
* it does not accidentally destroy any files. We check the following
* - That we can  create an arbritray random file and write to it
* - That  we return value is false when opening an existing file for writing
* - That we can open an existing file for reading */
TEST_F(TestGFileIOHandler, CheckFileNSR305)
{
    GFileIOHandler *f = g_file();
    
    string rand_fname1 = g_random()->Name("testfile", ".txt");
    string rand_fname2 = g_random()->Name("testfile", ".txt");
    ASSERT_NE(rand_fname1, rand_fname2 );

    // For a non existing file we expect the return value to be true if we try to create it for writing, and false othervise 
     EXPECT_EQ(true,  f->CheckFile(rand_fname1, "w"));
     EXPECT_EQ(true,  f->CheckFile(rand_fname1, "w+"));
     EXPECT_EQ(true,  f->CheckFile(rand_fname1, "a"));
     EXPECT_EQ(true,  f->CheckFile(rand_fname1, "a+"));
     EXPECT_EQ(false, f->CheckFile(rand_fname1, "r"));
     EXPECT_EQ(false, f->CheckFile(rand_fname1, "r+"));


    FILE *fp = f->OpenFile(rand_fname2, "w", GLOCATION );
    ASSERT_NE(fp, nullptr);

    // We write an arbritray string to the file in order to check at the end that the UtilitesCheckFile function is non destructive
    fprintf(fp, "Hello Dolly\n");
    fclose(fp);
    
    // For an exisiting file we expect the return value to be false if we try to open it with the w/w+ option because data
    // allready in the file will then be lost, for all other valid access options we expect true as the retrun value
    EXPECT_EQ(false, f->CheckFile(rand_fname2, "w"));
    EXPECT_EQ(false, f->CheckFile(rand_fname2, "w+"));
    EXPECT_EQ(true,  f->CheckFile(rand_fname2, "a"));
    EXPECT_EQ(true,  f->CheckFile(rand_fname2, "a+"));
    EXPECT_EQ(true,  f->CheckFile(rand_fname2, "r"));
    EXPECT_EQ(true,  f->CheckFile(rand_fname2, "r+"));

    // Checking that the file is intact after all the testing
    EXPECT_TRUE ( f->Delete(rand_fname2) );

    #ifdef HAS_LOGGING
    //EXPECT_EQ("Hello Dolly", FileIOTest(rand_fname2));
    vector<string> invalidoptions = { "b", "c", "d", "e", "f", "g", "h", "i", "j",
        "k", "l", "m", "n", "o", "p", "q", "s", "t",
        "u", "v", "x", "y", "z" };

    for (size_t i = 0; i < invalidoptions.size(); i++)
    {
        EXPECT_EQ(false, f->CheckFile("doesntmatter.txt", invalidoptions[i].c_str()));
    }
    #endif

}



TEST_F(TestGFileIOHandler, AppendCreate)
{
    GFileIOHandler* f = g_file();
    string fname = g_random()->Name("append_test", ".txt");
    EXPECT_EQ(true, f->Append(fname, "testwrite to file with parameters: a=%d, b=%d\n", 42, 43));
    
    #ifdef HAS_LOGGING
    EXPECT_EQ(FileIOTest(fname), "testwrite to file with parameters: a=42, b=43");
    #endif

    EXPECT_EQ (true,   f->Delete(fname));
    EXPECT_EQ( false,  f->Delete(fname));
    EXPECT_EQ( false,  f->Delete(fname));
    fname = g_random()->Name("append_test", ".txt");
    EXPECT_EQ(true,  f->CreateFileLocal(fname));
    
    //EXPECT_EQ(false, f->CreateFileLocal(fname));

    EXPECT_EQ(true,  f->Delete(fname));
    EXPECT_EQ(false, f->Delete(fname));
}



TEST_F(TestGFileIOHandler, Read)
{
    GFileIOHandler* f = g_file();
    string fname = g_random()->Name("read_test", ".txt");
    f->Append(fname,  "Lorem ipsum dolor sit amet, consectetur adipiscing elit");
    vector<string> r = f->ReadAll(fname);
    EXPECT_EQ(1, r.size());
    
    if (r.size() == 1)
    {
        EXPECT_EQ("Lorem ipsum dolor sit amet, consectetur adipiscing elit", r[0]);
    }

    f->Append(fname, "\nsed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
    f->Append(fname, "\nUt enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat");

    r = f->ReadAll(fname);
    EXPECT_EQ(3, r.size());
   
    if (r.size() == 3)
    {
        EXPECT_EQ(r[1], "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
        EXPECT_EQ(r[2], "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat");
    }

    f->Delete(fname);

    fname = g_random()->Name("read_test", ".txt");
    f->Append(fname,  "Duis aute irure dolor in reprehenderit in voluptate\n\
velit esse cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat\n\
cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");


    r = f->ReadAll(fname);
    EXPECT_EQ(3, r.size());

    
    if (r.size() == 3)
    {
        EXPECT_EQ(r[0], "Duis aute irure dolor in reprehenderit in voluptate");
        EXPECT_EQ(r[1], "velit esse cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat");
        EXPECT_EQ(r[2], "cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum." );
        EXPECT_EQ(f->ReadLastLine(fname),     "cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
        EXPECT_EQ(f->ReadLastLine(fname, 1),  "velit esse cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat");
        EXPECT_EQ(f->ReadLastLine(fname, 2),  "Duis aute irure dolor in reprehenderit in voluptate");
        EXPECT_EQ(f->ReadLastLine(fname, 10), "Duis aute irure dolor in reprehenderit in voluptate");
        //   EXPECT_ANY_THROW(f->ReadLastLine(fname, -1)) ;
        EXPECT_EQ(f->ReadFirstLine(fname), "Duis aute irure dolor in reprehenderit in voluptate");
    }
    

    f->Delete(fname);
}



#ifdef _WIN32
TEST_F(TestGFileIOHandler, GetAbsolutePath)
{
    GFileIOHandler *f = g_file();
    string cwd1 = f->GetAbsolutePath(".");
    char cwd2[PATH_MAXLENGTH];
    GetCurrentDirectoryA( PATH_MAXLENGTH, cwd2);
    EXPECT_EQ(cwd1, cwd2);
}
#endif


TEST_F(TestGFileIOHandler, GetExtention)
{
   GFileIOHandler *f = g_file();
    string exeExt = f->GetExtention(g_system()->GetExeName());
#ifdef _WIN32
    EXPECT_EQ(exeExt, "exe");
#else
    EXPECT_EQ(exeExt, "");
#endif
}

