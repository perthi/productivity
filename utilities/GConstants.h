// -*- mode: c++ -*-
#ifndef CONSTANTS_H
#define CONSTANTS_H

/****************************************************************************
*** Copyright(C) 2015  Per Thomas Hille, pth@embc.no                     ***
*** This file is part of logmaster. logmaster is free software : you can ***
*** redistribute it and / or modify it under the terms of the Lesser GNU ***
*** General Public License (LGPL) V3 or later. See .cpp file for details ***
*****************************************************************************/


/** @brief Common constants used by the commandline and logging system*/
namespace GCONSTANTS
{
  static const bool fgkMANDATORY = true;
  static const bool fgkOPTIONAL = false; 
  static const int fgkPASSED = 0;
  static const int fgkWARNING = 1;
  static const int fgkFAILED = 2;
  static const int fgkDISABLED = 3;
  static const int fgkINVALID_TEST = 4;
  static const int fgkINVALID_PARAMETER = 5;
  static const int fgkINVALID = 6;

  static const bool fgkIGNORE_CASE = true;
  static const bool fgkCASE_SENSITIVE = false;

}

#endif
