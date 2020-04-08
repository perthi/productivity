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
///#include  "GException.h"
#include "GCommon.h"
#include "GText.h"

#include "GLocation.h"

#include <string>
#include <random>
#include <typeinfo>

#include <limits.h>

using std::random_device;
using std::string;


class GRandom;

inline  GRandom * g_random();

/** @brief Interface for random number generation from a uniform, Gaussian or binominal distribution  */
class GRandom
{
public:
    GRandom() {};
    ~GRandom() {};
    inline string   Name(const string prefix = "", const string postfix = "");
    inline string   RandomString( const size_t size  );

    template<typename T> T
    inline Binominal(T n, double p, typename std::enable_if<std::is_integral<T>::value>::type* = 0)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::binomial_distribution<> d(n, p);
        return d(gen);
    }


    template<typename T>  T
    inline  Binominal(T n, double p, typename std::enable_if<std::is_floating_point<T>::value>::type* = 0)
    {
        g_common()->HandleError(  GText( "The parameter n mus be an integral value, n is of type %s", typeid(n).name()).str(), GLOCATION, THROW_EXCEPTION  );
        return 0;
    }
  

    template< typename T>   T
    inline  Uniform(T  min, T max, typename std::enable_if<std::is_floating_point<T>::value>::type* = 0)

    {
        if (min > max)
        {
            g_common()->HandleError(  GText(  "Min value must be lower than max value, you have entered  min = %f and max = %f", min, max  ).str(), GLOCATION, THROW_EXCEPTION  );
            return -1;
        }
        else
        {
            static std::default_random_engine generator;
            static std::mt19937 gen(generator());
            std::uniform_real_distribution<T> uniform_dist(min, max);
            T tmp =   uniform_dist(generator);
            return  tmp;
        }
        return 0;
    }


    template< typename T>  T
    inline  Uniform(T min, T max, typename std::enable_if<std::is_integral<T>::value>::type* = 0)
    {
        if (  (min) > (max)  )
        {
            g_common()->HandleError(  GText(  "Min value must be lower than max value, you have entered  min = %d and max = %d", min, max  ).str(), GLOCATION, false  );
            return -1;
        }
        else
        {
            static std::random_device rd;  //Will be used to obtain a seed for the random number engine
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<T>  uniform_dist(min, max);
            return  uniform_dist( gen );
        }
    }  
    

    template<typename T>  T
    inline   Gauss(T mean, T sigma, typename std::enable_if<std::is_floating_point<T>::value>::type* = 0)
    {
        T num = -9999;
        if (sigma < 0)
        {
            ///RANGE_EXCEPTION("Invalid sigma value %f, sigma must be positive", sigma);
            g_common()->HandleError(  GText(  "Invalid sigma value %f, sigma must be positive" ) .str(), GLOCATION, THROW_EXCEPTION  );
        }
        else
        {
            static std::random_device rd;  //Will be used to obtain a seed for the random number engine
            static std::mt19937 gen(rd());
            std::normal_distribution<T> gauss_dist(mean, sigma);
            num = gauss_dist(gen);
        }
        return num;
    }
    
    template<typename T>  T
    inline  Gauss(T /*mean*/, T /*sigma*/, typename std::enable_if<std::is_integral<T>::value>::type* = 0)
    {
        //EXCEPTION("You cannot use the Gauss random geneartor with an integral typ. %s Is an integral type, use a binominal generator instead", typeid(T).name());
        g_common()->HandleError(  GText(  "You cannot use the Gauss random geneartor with an integral type. \
                                           %s Is an integral type, use a binominal generator instead", typeid(T).name()) .str(), GLOCATION,   THROW_EXCEPTION  );
        return 0;
    }
};


//#ifdef _WIN32
inline string
GRandom::Name(const string prefix, const string postfix)
{
	static char name[4096];
	unsigned short num = Uniform<unsigned short>(0, USHRT_MAX);
	SPRINTF(name, 4096 - 1, "%s%05d%s", prefix.c_str(), num, postfix.c_str());
	//    sprintf(name, "%s%05d%s", prefix.c_str(), num, postfix.c_str() );
	return string(name);
}
//#endif


inline string   
GRandom::RandomString( const size_t size  )
{
    string tmp = "";

    for(size_t i = 0; i <  size; i++ )
    {
        char c = Uniform<int>(0, 255);
        tmp.push_back(c);
    }
    return tmp;
}


inline  GRandom  * g_random()
{
	static GRandom *instance = new GRandom();
	return instance;
}

