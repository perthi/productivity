// -*- mode: c++ -*-

/*****************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                       ****
******************************************************************************/

/*****************************************************************************
******************************************************************************
*** This file is part of logmaster.                                        ***
*** Copyright (C) 2018 Per Thomas Hille <pth@embc.no>  http:///www.embc.no ***
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


#include "GCrc.h"
#include "GCommon.h"
#include "GText.h"
#include "GLocation.h"



uint32_t GCrc::icrctb[256];


GCrc::GCrc(const int32_t idx, const bool fill) : fJfill(fill ? 255 : 0), fJcrc(0), fPoly(0)
{
    int32_t j;
    ///uint32_t fOkpolys[8] = {0x755B, 0xA7D3, 0x8005, 0x1021, 0x5935, 0x90D9, 0x5B93, 0x2D17};
    fPoly = fOKpolys[idx & 7];
    fPolyIndex = idx;

    for (j = 0; j < 256; j++)
    {
        icrctb[j] = Icrc1(j << 8, 0);
    }

    fJcrc = (fJfill | (fJfill << 8));
}


#if defined( __unix__ )  || defined (_WIN32 )
uint16_t
GCrc::Crc(const string &bufptr)
{
    fJcrc = (fJfill | (fJfill << 8));
    return Concat( (uint8_t *)bufptr.c_str(), bufptr.size() );
}
#endif


uint16_t
GCrc::Crc(const uint8_t *in, const uint16_t len )
{
    fJcrc = (fJfill | (fJfill << 8));
    return Concat( in, len );

}


#if defined( __unix__ )  || defined (_WIN32 )
void
GCrc::AddCrc16( string &  in  )
{
    uint16_t size_out = 0;

    // CRAP PTH, magic number
    if( in.size() < 1024)
    g_common()->HandleError(  GText(   "input buffer too large"  ).str(), GLOCATION, THROW_EXCEPTION  );

    uint8_t buf[1024];

    for(uint16_t i = 0; i < in.size(); i++  )
    {
        buf[i] = in.at(i);
    }

    AddCrc16( buf, in.size(), &size_out);
    in.erase(in.begin(), in.end() );

    for(uint16_t  i= 0; i < size_out; i++ )
    {
        in.push_back( buf[i] );
    }

}
#endif



void
GCrc::AddCrc16( uint8_t * in, const uint16_t input_size, uint16_t *output_size )
{
    const unsigned short crc_i =   Crc(in, input_size);
    char lo = LoByte(crc_i);
    char hi = HiByte(crc_i);

    in[input_size] = hi;
    in[input_size +1 ] = lo;

    *output_size =  input_size + 2;
}


#if defined( __unix__ )  || defined (_WIN32 )
bool
GCrc::CheckCrc( string &in )
{
    return CheckCrc( (const uint8_t *)in.c_str(), in.size() );
}
#endif


bool
GCrc::CheckCrc(  const uint8_t *in, const uint16_t total_size  )
{
    int val = Crc( in, total_size );
    return val == 0 ? true : false;
}



uint16_t
GCrc::Concat(const  uint8_t *bufptr, const uint16_t len )
{
    uint32_t j, cword = fJcrc;

    for (j = 0; j < len; j++)
    {
        cword = icrctb[uint8_t(bufptr[j]) ^ HiByte(cword)] ^ ( LoByte(cword) << 8);
    }

    return fJcrc = cword;
}


uint16_t
GCrc::Icrc1(const uint16_t jcrc, const uint8_t onech)
{
    //		Int i;
    uint16_t ans = (jcrc ^ onech << 8);

    for (int i = 0; i < 8; i++)
    {
        if (ans & 0x8000)
        {
            // ans = ( (ans <<= 1) ^  fPoly );
            ans <<= 1;
            ans = ans ^ fPoly;
        }
        else
        {
            ans <<= 1;
            ans &= 0xffff;
        }
    }
    return ans;
}


uint8_t
GCrc::LoByte(const uint16_t x)
{
    return (uint8_t)(x & 0xff);
}


uint8_t
GCrc::HiByte(const uint16_t x)
{
    return (uint8_t)((x >> 8) & 0xff);
}


