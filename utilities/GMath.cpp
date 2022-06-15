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



#include "GMath.h"
#include <cmath>
#include "GTime.h"

#ifdef _WIN32
#include <corecrt_math_defines.h>
#endif


double
GMath::Sine(const double A, const double f, const double omega,  double *deltatime )
{
    static GTime time;
    static double start_time = time.GetEpochTime();
    static double _2pix = M_PI * 2;
    // static int  f = 100;
    double delta_time = time.GetEpochTime() - start_time;
    if (deltatime != nullptr)
    {
        *deltatime = delta_time;
    }
    return A * sin(_2pix * f * delta_time + omega);
}
