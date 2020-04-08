// -*- mode: c++ -*-

#ifndef GCRC_H
#define GCRC_H

/****************************************************************************
*** Copyright(C) 2015  Per Thomas Hille, pth@embc.no                     ***
*** This file is part of logmaster. logmaster is free software : you can ***
*** redistribute it and / or modify it under the terms of the Lesser GNU ***
*** General Public License (LGPL) V3 or later. See .cpp file for details ***
*****************************************************************************/


#include "GDefinitions.h"
#include <stdint.h>

#define DFAULT_POLY_INDEX 2

class GCrc 
{   
public:
	GCrc(const int32_t jpoly =DFAULT_POLY_INDEX, const bool fill=true);
	#if defined( __unix__ )  || defined (_WIN32 )
	uint16_t     Crc(const string &bufptr);
	void         AddCrc16( string &in );
	bool 		 CheckCrc( string &in );
	#endif

	uint16_t     Crc(const uint8_t *in, const uint16_t len );
	void         AddCrc16(  uint8_t * in, const uint16_t input_size, uint16_t *output_size );
	bool 		 CheckCrc(  const uint8_t *in, const uint16_t total_size  );

	uint8_t      LoByte( const uint16_t x ); 
    uint8_t      HiByte( const uint16_t x );

private:
	//uint16_t     Concat(const string &bufptr);
	uint16_t     Concat( const uint8_t *bufptr, const uint16_t len );
	uint16_t     Icrc1(  const uint16_t jcrc, const uint8_t onech ); 
	
	uint32_t fJfill, fJcrc,fPoly;
	static uint32_t icrctb[256];
	uint32_t fOKpolys[8] = {0x755B, 0xA7D3, 0x8005, 0x1021, 0x5935, 0x90D9, 0x5B93, 0x2D17};
	int fPolyIndex = DFAULT_POLY_INDEX;	
};



#endif
