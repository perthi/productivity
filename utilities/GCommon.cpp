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

#include <utilities/GCommon.h>

#include "GDefinitions.h"
#include "GLocation.h"

#ifdef HAS_LOGGING
#include <logging/LLogApi.h>
using namespace LOGMASTER;
#endif

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;



GCommon * g_common()
{
	static GCommon *instance = new GCommon();
	return instance;
}



#ifdef HAS_LOGGING
void
GCommon::HandleError(const string message, const GLocation  l,   const bool   disable_exception  )
{
    if ( disable_exception == false)
    {
        throw_exception(   GException(l.fFileName, l.fFunctName, l.fLineNo, SYS_EX, "%s", message.c_str( ))  )  ;
    }
    else
    {
        G_WARNING(message.c_str());
    }
}

#else

void
GCommon::HandleError(const string message, const GLocation l, const bool   disable_exception  )
{
    if ( disable_exception == false)
    {
    #ifdef _WIN32
		throw( std::exception(message.c_str() ) );
	#else
		throw(std::invalid_argument( message.c_str() ));
	#endif
    }
    else
    {
        cout << l.fFileName << "::" << l.fFunctName << ", line " << l.fLineNo << ";" << message << std::endl;
    }
}

#endif



