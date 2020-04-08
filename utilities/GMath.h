// -*- mode: c++ -*-
#pragma once

/***************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                     ****
****************************************************************************/

/****************************************************************************
*** Copyright(C) 2018  Per Thomas Hille, pth@embc.no                      ***
*** This file is part of logmaster.logmaster is free software : you can   ***
*** redistribute it and / or modify it under the terms of the Lesser GNU  ***
*** General Public License(LGPL) V3 or later.See.cpp file for details     ***
*****************************************************************************/

#include "GDefinitions.h"
#include <typeinfo>
#include <type_traits>
#include <cmath>
#include <limits>



class GMath;

inline GMath * g_math();

/** @brief various math functions */
class GMath
{
public:
	GMath() {};
	~GMath() {};

	inline double               GoldenLongEdge(const double shortedge = 1); // Golden cut
    inline double               GoldenShortEdge( const double longedge  = 1 );  // Golden cut 
	template<typename T>  inline T     Min(T val1, T val2);
	template<typename T>  inline T     Max(T val1, T val2);
	template<typename T>  inline T     Abs(const T in);

#ifdef _WIN32 
    template< typename T = std::enable_if< std::is_fundamental<T>::value, T >::type  >
#else
   template< typename T  >
#endif
  inline bool    IsEqual(const T val1, const T val2, const double tolerance = -1)
  {
	  if (tolerance >= 0)
	  {
		  return std::fabs(val1 - val2) <= tolerance;
	  }
	  else
	  {
		  return std::fabs(val1 - val2) <= std::numeric_limits<T>::epsilon() ? true : false;
	  }
  }
  double  fgkGoldenRatio = (1 + sqrt(5)) / 2;
	

};




inline GMath * g_math()
{
	static GMath *instance = new GMath();
	return instance;
}







/** The golde ration. The funtion takes as input the short side of a rectangle that you want to have
* the golde ration, an returns the length of the long edge.
* @param  shortedge  The short side of the rectangle with the golden ratio
* @return The long side of the rectangle such that it has the golden ratio */
double
GMath::GoldenLongEdge(const double shortedge)
{
	return shortedge*fgkGoldenRatio;
}


/** The golde ration. The funtion takes as input the long side of a rectangle that you want to have
* the golde ration, an returns the length of the short edge.
* @param  longedge  The short side of the rectangle with the golden ratio
* @return The short side of the rectangle such that it has the golden ratio */
double
GMath::GoldenShortEdge(const double longedge)
{
	return longedge / fgkGoldenRatio;
}




template<typename T> T
GMath::Max(T val1, T val2)
{
    return val1 > val2 ? val1 : val2;
}


template<typename T> T
GMath::Min(T val1, T val2)
{
    return val1 < val2 ? val1 : val2;
}


/** @return The absolute value of "in" */
template<typename T>
T
GMath::Abs(const T in)
{
    return (in > 0) ? in : -1 * in;
}


