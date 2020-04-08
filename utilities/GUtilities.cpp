// -*- mode: c++ -*-


/*****************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                       ****
******************************************************************************/

/*****************************************************************************
******************************************************************************
*** This file is part of logmaster.                                        ***
*** Copyright (C) Per Thomas Hille <pth@embc.no>  http:///www.embc.no      ***
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


#include "GUtilities.h"
#include "GTokenizer.h"

///#include <logging/LLogging.h>


#ifdef _WIN32
#include <Windows.h>
#endif

#ifndef _WIN32
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#endif

////using namespace LOGMASTER;


#include <regex>


GUtilities * g_utilities()
{
    static  GUtilities * instance = new GUtilities();
    return instance;
}


void 
GUtilities::Sizes()
{
    short s = 0;
    int i = 0;
    long int li = 0;
    unsigned long int uli = 0;
    unsigned long long int ulli = 0;
    double d = 0;
    long double ld = 0;
    float f = 0;
    cout << "sizeof(short) = \t\t"    << sizeof(s)    << endl;
    cout << "sizeof(int) =\t\t\t"    << sizeof(i)    << endl;
    cout << "sizeof(long int) =\t\t"   << sizeof(li)   << endl;
    cout << "sizeof(unsigned long int) =\t"  << sizeof(uli)  << endl;
    cout << "sizeof(uns. long long int )=\t" << sizeof(ulli) << endl;
    cout << "sizeof(double) =\t\t"    << sizeof(d)    << endl;
    cout << "sizeof(long double) =\t\t"   << sizeof(ld)   << endl;
    cout << "sizeof(float) =\t\t\t"    << sizeof(f)  << endl;
}


void
GUtilities::DisableError()
{
	fIsDisabledError = true;
}


void
GUtilities::EnableError()
{
	fIsDisabledError = false;
}


bool
GUtilities::IsDisabledError()
{
	return fIsDisabledError;
}


bool   
GUtilities::IsLittleEndian()
{
   int num = 1;

    if (*(char *)&num == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool    
GUtilities::IsBigEndian()
{
    return IsLittleEndian() == true ? false : true;
}    

#ifdef _WIN32
string 
GUtilities::QueryInput( const string prompt)
{
    cout << prompt;	
    string user_input;
    std::getline(std::cin,  user_input);
    return user_input;
    //double freq_f = g_numbers()->ToFloat( freq);
}




#else
string  
GUtilities::QueryInput(const string prompt)
{
    string option;

    const char *line;
    line = readline(prompt.c_str() );

    if (line != nullptr)
    {
        add_history(line);
        option = string(line);
        free((void *)line);
    }
    return option;
}
#endif



bool 
GUtilities::IsValidIPV4Address(const string ipv4_address)
{
    std::regex pattern("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
    return std::regex_match(ipv4_address, pattern);
}



string     
GUtilities::Copy(const char *buffer, const int length, string *in)
{
    if(in != nullptr )
    {
        in->clear();
    }

    string tmp;

    for(int i=0; i < length; i++ )
    {
        tmp.push_back(buffer[i]);
    }

    if( in != nullptr)
    {
        *in = tmp;
    }

    return tmp;
}

