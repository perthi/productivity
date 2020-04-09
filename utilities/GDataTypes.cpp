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


#include "GNumbers.h"
#include "GDataTypes.h"
#include "GCommon.h"
#include "GText.h"
#include "GLocation.h"
#include "GStackTrace.h"

ostream& operator<<(ostream& os, const Val  &o)
{
    os << o.GetValue(  );   
    return os;
}


void 
Val::CheckIsInteger(double t)
{
    if( g_numbers()->IsInteger((long double )t) == false )
    {
        g_common()->HandleError( GText( "Number (%f) is NOT an integer, the sensor ID must be an integer between ZERO and %d", t, 16).str(), 
                                         GLOCATION, THROW_EXCEPTION  );
    }
}


void 
Val::GeneratStackFrames()
{
    string stack =  GStackTrace::str();
    std::stringstream msg;
    msg <<"The allowed range for parameter: "<< fName <<"\tis  [min, max] = "<< "["<< fMinValue <<", "<< fMaxValue <<"]  " << fSubscript;	
    msg << ":\tYou attempted to set the value to " << fVal;  
    string s = stack + msg.str() ;
    g_common()->HandleError( s, GLOCATION, THROW_EXCEPTION );
}


void
Val::CheckLimits(const double &t, const double min, const double max)
{
    if(t > max || t < min)
    {
	    GeneratStackFrames();
    }
}


void 
Val::SetValue(const double value) 
{ 
    fVal = value; 
    CheckLimits( value, fMinValue, fMaxValue );
}
