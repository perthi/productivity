// -*- mode: c++ -*-
#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

/***************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                     ****
****************************************************************************/

/****************************************************************************
*** Copyright(C) 2018  Per Thomas Hille, pth@embc.no                      ***
*** This file is part of logmaster.logmaster is free software : you can   ***
*** redistribute it and / or modify it under the terms of the Lesser GNU  ***
*** General Public License(LGPL) V3 or later.See.cpp file for details     ***
*****************************************************************************/

/*Color Definitions using Ansi colors*/
#define A_WHITE      97
#define A_BLACK      30
#define A_GREEN      92
#define A_DARK_GREEN 32
#define A_YELLOW     93
#define A_ORANGE     33
#define A_RED		 31
#define A_BLUE		 34 
#define A_LIGHT_BLUE 36
#define A_CYAN       96
#define A_PURPLE	 95
#define A_GRAY       37

/* Color defineitions usng RGB format*/
#define C_WHITE      0xFFFFFF
#define C_BLACK      0x000000
#define C_GREEN      0x28ED4D
#define C_DARK_GREEN 0x256E53
#define C_YELLOW     0xF0F70C
#define C_ORANGE     0xEDAF28
#define C_RED		 0xED3C28
#define C_BLUE		 0x1313F0 
#define C_LIGHT_BLUE 0x6ABCE6
#define C_CYAN       0x28EDE0
#define C_PURPLE	 0x7E28ED
#define C_GRAY       0xCFCFCF
#define C_LIGHT_GRAY 0xD8D8D8

/*color definitions for the windows terminal/console
* Here we dont have much resolution/bits to play with, but lets give it a try */
#ifdef _WIN32
#define T_DARK_GREEN  ( FOREGROUND_GREEN )
#define T_GREEN       ( FOREGROUND_GREEN | FOREGROUND_INTENSITY )
//#define T_ORANGE      ( FOREGROUND_GREEN | FOREGROUND_RED  | FOREGROUND_INTENSITY )
#define T_ORANGE      ( FOREGROUND_GREEN | FOREGROUND_RED )
#define T_RED         ( FOREGROUND_RED   | FOREGROUND_INTENSITY ) 
#define T_PURPLE      ( FOREGROUND_BLUE  | FOREGROUND_RED | FOREGROUND_INTENSITY )
#define T_WHITE       ( FOREGROUND_BLUE  | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY )
#define T_YELLOW      ( FOREGROUND_GREEN | FOREGROUND_RED |  FOREGROUND_INTENSITY )
#define T_BLUE        ( FOREGROUND_BLUE  | FOREGROUND_INTENSITY )
#define T_GRAY		  ( FOREGROUND_INTENSITY )
#define T_LIGHT_GRAY  ( FOREGROUND_RED  | FOREGROUND_BLUE | FOREGROUND_GREEN )
#define T_CYAN        ( FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY )
#define T_PURPLE      ( FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY )
#else
#define T_DARK_GREEN C_DARK_GREEN
#define T_GREEN      C_GREEN
#define T_ORANGE     C_ORANGE
#define T_RED        C_RED
#define T_PURPLE     C_PURPLE
#define T_WHITE      C_WHITE
#define T_YELLOW     C_YELLOW
#define T_BLUE       C_BLUE
#define T_GRAY	     C_GRAY	
#define T_LIGHT_GRAY C_LIGHT_GRAY 
#define T_CYAN       C_CYAN
#define T_PURPLE     C_PURPLE

#endif


#define CONSOLE_DEFAULT T_LIGHT_GRAY

