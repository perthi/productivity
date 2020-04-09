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


#include "TestGSystem.h"
#include <utilities/GSystem.h>
#include <utilities/GRandom.h>
#include <utilities/GFileIOHandler.h>
#include <utilities/GCommon.h>
#include <utilities/GLocation.h>
#include <utilities/GText.h>



TestGSystem::TestGSystem()
{
}


TestGSystem::~TestGSystem()
{
}

 
TEST_F(TestGSystem, mkfile)
{
    try
    {
        g_system()->mkfile("testdir/testfile.txt");
        EXPECT_EQ(0, g_system()->rm( "testdir/testfile.txt") ) ;

    }
    catch( std::exception &e  )
    {
        g_common()->HandleError( e.what(), GLOCATION, DISABLE_EXCEPTION );

    }
    #ifdef HAS_LOGGING
    catch (GException &e)
    {
        cerr << e.what() << endl;
    }
    #endif
    catch(...)
    {
        g_common()->HandleError( "Unknown exception caucht", GLOCATION, DISABLE_EXCEPTION );
    }
}



TEST_F(TestGSystem,  cp)
{
    string s = "source.txt";
    string d = "dest.txt";
    g_system()->rm(s);
    g_system()->rm(d);

    g_system()->mkfile(s);
    g_file()->Append(s,  "Lorem ipsum dolor sit amet");
    g_file()->Append(s,  "\nconsectetur adipiscing elit");
  
    g_system()->cp(s, d);
    
    vector<string> content = g_file()->ReadAll(d);
    EXPECT_EQ(2, content.size());

    if (content.size() == 2)
    {
        EXPECT_EQ("Lorem ipsum dolor sit amet",  content[0]);
        EXPECT_EQ("consectetur adipiscing elit", content[1]);
    }
    
    EXPECT_TRUE(g_file()->CheckFile(s));
    EXPECT_TRUE(g_file()->CheckFile(d));

    g_system()->rm(s);
    g_system()->rm(d);
}


TEST_F(TestGSystem, mv)
{
    string s = "source.txt";
    string d = "dest.txt";
    g_system()->rm(s);
    g_system()->rm(d);

    g_system()->mkfile(s);
    g_file()->Append(s, "Sed ut perspiciatis unde omnis iste natus error sit voluptatem");
    g_file()->Append(s, "\naccusantium doloremque laudantium, totam rem aperiam, eaque ipsa");

    g_system()->mv(s, d);

    vector<string> content = g_file()->ReadAll(d);
    EXPECT_EQ(2, content.size());

    if (content.size() == 2)
    {
        EXPECT_EQ("Sed ut perspiciatis unde omnis iste natus error sit voluptatem", content[0]);
        EXPECT_EQ("accusantium doloremque laudantium, totam rem aperiam, eaque ipsa", content[1]);
    }

    EXPECT_FALSE(g_file()->CheckFile(s));
    EXPECT_TRUE(g_file()->CheckFile(d));
    g_system()->rm(d);
}

