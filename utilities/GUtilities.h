// -*- mode: c++ -*-

// #ifndef GUtilities_HPP
// #define GUtilities_HPP

#pragma once

/****************************************************************************
*** Copyright(C) 2015  Per Thomas Hille, pth@embc.no                     ***
*** This file is part of logmaster. logmaster is free software : you can ***
*** redistribute it and / or modify it under the terms of the Lesser GNU ***
*** General Public License (LGPL) V3 or later. See .cpp file for details ***
***
**************************************************************************/

//#define  G_STANDALONE

//#ifndef G_STANDALONE
//#include <logging/LLogApi.h>
//#include "GException.h"
//using namespace LOGMASTER;
//#endif


#include "GDefinitions.h"
#include "GLocation.h"
#include "GCommon.h"
#include "GText.h"

#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <stdint.h>
#include <climits>
#include <type_traits>
#include <typeinfo>
#include <set>


//using std::stringstream;
using std::ostream;
using std::ostringstream;
using std::vector;
using std::string;
using std::dec;
using std::map;
using std::set;

using namespace std; // CRAP PTH

#define PRINT( array ) g_utilities()->Print( array, __FILE__, __func__,  __LINE__ )
#define ABS(a) g_utilities()->Abs(a)
#define CHECKLIMITS(value,lower,upper, status) g_utilities()->CheckLimits(value,lower,upper,#value,  __FILE__, __LINE__, __FUNCTION__, status)

class GUtilities;


GUtilities * g_utilities();


/** @brief Collection of various utility functions that does not fit into any other category  */
class  GUtilities
{
    friend GUtilities * g_utilities();
public:
  
    string		        API     Copy(const char *buffer, const int length, string *in = nullptr);  
    string				API		QueryInput( const string prompt);
    bool				API     IsLittleEndian();
    bool				API     IsBigEndian();       
    void				API		Sizes();
    void				API		DisableError();
    void				API		EnableError();
    bool				API		IsDisabledError();
    bool		 	    API     IsEmpty(const string &in);
    
    template <typename T >
    inline void ResetArray( T *arr, const size_t size ) const;

    template < typename T >   void               Print(const T &array, const string file, const string func, const int line );
    template < typename T >   void               Print(vector<T> in);
    template < typename T >   vector<string>     Append(vector<T> &original, const vector<T> appendix );
    template < typename T >   string	           CheckLimits(const T val, const T low, const T up, const char *varname, const char * filename, const  int linenumber, const char * functionname, bool *status = nullptr);
    template < typename T >   bool               IsInRange(const T value, const T lower, const T upper);
    template < typename T1,   typename T2>      bool  Contains(map<T1, T2> *, T1);
    template < typename T >   string             Vec2String(const vector<T>, const string sep =  "\n");
    template < typename T>								 string		Hash2String( const map< string, T>   *m, const int ncols = 8, const string sep = "");
    template < typename T1,	  typename T2>				 vector< T1>	Hash2StringV(const map< T1, T2>  *m);
	template < typename T1,   typename T2>				 vector< T2>	Hash2SContentV(const map< T1, T2>* m);
    template < typename T >   bool               HasElement(const T element, const vector<T> &in);
    template < typename T >   bool               Bits2Array(const T element, vector<T> &in);
    template < typename T >	  bool               Array2Bits(T & element, const vector<T>& in);
    template < typename T>    bool               Contains(const vector<T> vect, const T element);
    template < typename T1, typename T2>   bool  CheckMinMax(const T1 min, const T2 max);
    template < typename T>				   void FilterOut(vector<T> &in,  const vector<T> &filter);
    string     API   TabAlign(const string &in,  int max_tabs = 4, int *n_tabs = nullptr );

	/// template <typename T1, typename T2>   bool  AddUniqueElelement(std::map<T1, T2>& m, T1);
    bool      API  IsValidIPV4Address(const string ipv4_address);


private:
    GUtilities() {};
    virtual ~GUtilities() {};   
    bool fIsDisabledError = false;
    char fCurDir[1024] = "";
};





template <typename T >
void 
GUtilities::ResetArray( T *arr, const size_t size ) const
{
    size_t size_l = size/(sizeof(T));

    for(size_t i = 0; i < size_l; i ++ )
    {
        arr[i] = 0;
    }
}





template<typename T>  vector<string>  
GUtilities::Append(vector<T> &original, const vector<T> appendix )
{
    original.insert(original.end(), appendix.begin(), appendix.end() );
    return original;
}


/**  Check if a values (val) is
 *
 */
template<typename T>
inline string
GUtilities::CheckLimits(const T val, const T low, const T up,  const char * varname, const char * filename, const  int linenumber, const char * functionname, bool *status )
{
    std::ostringstream l_msg;
    bool ret = false;

    if (val < low || val > up)
    {
        l_msg << varname << " is:" << val << " which is outside the range [" << low << ", " << up << "] ";
        
        if (status != nullptr)
        {
            *status = ret;
        }   
        
        g_common()->HandleError(l_msg.str(), GLocation(filename, linenumber, functionname), IsDisabledError());
    }
    else
    {
        if (status != nullptr)
        {
            *status = ret;
        }
    }


    return l_msg.str();

}



template<typename T>
inline bool
GUtilities::IsInRange(const T value, const T lower, const T upper)
{
    if (value >= lower && value <= upper)
    {
        return true;
    }
    else
    {
        return false;
    }
}


template<typename T>
void
GUtilities::Print(vector<T> in)
{
    for (size_t i = 0; i < in.size(); i++)
    {
        cout << in[i] << endl;
    }
}


/* Check wether or not the element T1 is contained in the map m */
template<typename T1, typename T2>
bool  
GUtilities::Contains(map<T1, T2> *m , T1 name)
{
    // typename std::map<T1, T2>::iterator it;
    auto it = m->find(name);
    return it == m->end() ? false : true;
}



template <typename T>
string
GUtilities::Vec2String(const vector<T> data, const string sep)
{
    ostringstream buffer;
    unsigned int n = (unsigned int)data.size();

    for (unsigned int i = 0; i < n; i++)
    {
        buffer << data[i];
      
        if (i < n - 1)
        {
            buffer << sep;
        }
    }
    return buffer.str();
}



/** Takes as input a hasmap and extracts the hash codes into a vector.
 *   For instance if the input is map<string, int> (i.e the hash code is a string)
 *   then the function returns a string with all the hash entries in the map. The string
 *   is formatted according to the prameters "ncols" and "sep"
 *   @tparam T1 the type of the hash entries (not used)
 *   @param[in] m The hash map to process
 *   @param[in]  ncols The number of colums printed to the output. 
 *   @param[in] sep   The separator used between each antry in a column. Default is "tab"
 *   @return a vector of hash codes/entries */
template <typename T>
string 
GUtilities::Hash2String(const map<string, T> *m, const int ncols, const string sep)
{
    std::stringstream buffer;

    int i = 0;

    for (auto iter = m->begin(); iter != m->end(); ++iter)
    {
        if (i % ncols == 0 && i != 0)
        {
            buffer << endl;
        
        }
        string tmp = iter->first;
        if (sep == "")
        {
            buffer << iter->first << (strlen(tmp.c_str()) < 8 ? "\t\t" : "\t");
        }
        else
        {
            buffer << iter->first << sep;
        }
        i++;
    }

    return buffer.str();
}


/** Takes as input a hasmap and extracts the
 *  hash codes into a vector. For instance if the input is map<string, int> (i.e the hash code is a string)
 *  then the return value is vector<string> with all entries in the map.
 *  @tparam T1 the hash code of the map
 *  @tparam T2 the element corresponding to T1
 *  @param[in] table The hash map to process
 *  @return either a vector of hash codes/entries */
template<typename T1, typename T2>
inline vector<T1> GUtilities::Hash2StringV(const map<T1, T2> * table)
{
    vector<T1> tmp;
    for (auto it = table->begin(); it != table->end(); it++)
    {
        tmp.push_back(it->first);
    }
    return tmp;
}


template<typename T1, typename T2>
inline vector<T2> GUtilities::Hash2SContentV(const map<T1, T2>* table)
{
	vector<T2> tmp;
	for (auto it = table->begin(); it != table->end(); it++)
	{
		tmp.push_back(it->second );
	}
	return tmp;
}


template<typename T>
inline bool GUtilities::HasElement(const T element, const vector<T>& in)
{
    for (size_t i = 0; i < in.size(); i++)
    {
        if (in[i] == element)
        {
            return true;
        }
    }
    return false;
}


template<typename T>
bool GUtilities::Bits2Array(const T element, vector<T>& in)
{
    static_assert(std::is_integral<T>::value, "Not an integral value.");
    if ((sizeof(element) * CHAR_BIT) == in.size())
    {
        T i(static_cast<T>(in.size()));
        for (T &bit : in)
        {
            bit = (element >> --i) & 0x01;
        }
        return true;
    }
    return false;
}


template<typename T>
bool GUtilities::Array2Bits(T& element, const vector<T>& in)
{
    static_assert(std::is_integral<T>::value, "Not an integral value.");

    if ((sizeof(element) * CHAR_BIT) == in.size())
    {
        T i(static_cast<T>(in.size()));
        for (T bit : in)
        {
            --i;
            if (bit == 1)
            {
                element += (bit & 0x01) << i;
            }
        }
        return true;
    }
    return false;
}



template<typename T>
bool GUtilities::Contains(const vector<T> vect, const T element)
{
    if (std::find(vect.begin(), vect.end(), element) != vect.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}


template<typename T1, typename T2>
bool GUtilities::CheckMinMax(const T1 min, const T2 max)
{
    std::stringstream buffer;

    if (min > max)
    {
        buffer << "min value is bigger than max value [min, max] = [" << min << ", " << max << " ]" << endl;
        g_common()->HandleError(buffer.str(), GLOCATION, IsDisabledError() );
        return false;
    }
    else
    {
        buffer << "[min, max] = [" << min << ", " << max << " ]" << endl;
        //G_DEBUG("%s", buffer.str().c_str());
        return true;
    }
}



template<typename T>
inline void 
GUtilities::FilterOut(vector<T>& in, const vector<T> &filter)
{
    /// CRAP PTH, bubble search
    for (size_t i = 0; i < filter.size() ; i++)
    {
        for ( size_t j = 0; j < in.size(); j++ )
        {
            if (in.at(j) == filter.at(i))
            {
                in.erase(in.begin() + j );
                j--;
            }
        }
    }
}

//template<typename T1, typename T2>
//inline void GUtilities::AddUniqueElelement(std::map<T1, T2>& m, T1)
//{
//}



/*********************************************************
**********************************************************
********************* OPERATORS **************************
**********************************************************
*********************************************************/
template <typename T>  
inline vector<T> operator -  (const vector<T> &lhs,  const vector<T> &rhs) 
{
    vector<T> tmp;
    
    if(lhs.size() != rhs.size() )
    {
        #ifndef G_STANDALONE
        EXCEPTION("Cannot evaluate the difference because the aray sizes differs (lhs.size() = %d, rhs.size() = %d)", 
                  lhs.size(), rhs.size());
        #else
        throw ( std::invalid_argument( "Cannot evaluate the difference because the aray sizes differs") );
        #endif

    }
    else
    {
	for( size_t i = 0; i < lhs.size(); i ++ )
	{
	    tmp.push_back(lhs[i] -rhs[i]);
	}
    }
    return tmp;
}


template <typename T1, typename T2>  
inline vector<T1> operator /  (const vector<T1> &lhs,  const T2 &rhs ) 
{
    vector<T1> tmp;
    
    if(rhs == 0 )
    {
        g_common()->HandleError(  "ATTEMP ON ZERO DIVISION", GLOCATION, THROW_EXCEPTION  );

    //  #ifndef G_STANDALONE    
	// EXCEPTION("ATTEMP ON ZERO DIVISION");
	//   #else
    //     throw ( std::invalid_argument( " ATTEMP ON ZERO DIVISION " ) );
    //     #endif
    return lhs;
    }
    else
    {
	for(int i = 0; i < lhs.size(); i ++ )
	{
	    tmp.push_back(lhs[i]/rhs);
	}
    }
    return tmp;
}


template <typename T1, typename T2>  
inline vector<T1> operator *  (const vector<T1> &lhs,  const T2 &rhs ) 
{
    vector<T1> tmp;
    
    for(int i = 0; i < lhs.size(); i ++ )
    {
	tmp.push_back(lhs[i]*rhs);
    }
    
    return tmp;
}


template<typename T>
inline ostream& operator << ( ostream& os,  const vector<T> &in )
{
    std::stringstream buffer;
    
    for(size_t i=0; i < in.size(); i++)
    {
	os << in[i] << endl;
    }
    
    return os;
}


template<typename T>
inline ostream& operator << ( ostream& os,  const vector<T *> &in )
{
    std::stringstream buffer;
    
    for(int i=0; i < in.size(); i++)
    {
	os << *in[i] << endl;
    }
    
    return os;
}


template<typename T>
inline void GUtilities::Print(const T &array, const string file, const string func, const int line)
{
    cout << file << ":" << line << ": " << func << ":  Printing array " << endl;

    for (int i = 0; i < array.size(); i++)
    {
        cout << array[i] << endl;
    }
}


/* We consider a string empty if it only contains spaces, tabs or newline *
 * in[in] The input string to check
 * @return true if the string is empty, false othervise */
bool
inline GUtilities::IsEmpty(const string &in)
{
    for (uint16_t i = 0; i < in.size(); i++)
    {
        if (in[i] != ' ' && in[i] != '\t' && in[i] != '\n')
        {
            return false;
        }
    }
    return true;
}


#define TAB_SIZE 8


inline string 
GUtilities::TabAlign( const string & in, int max_tabs, int *n  )
{
    //int n_tabs =  max_tabs - (int)(in.size() -1 )/ TAB_SIZE;
    int n_tabs =  max_tabs - (int)(in.size()  )/ TAB_SIZE;

    string tmp = in;
    
    if ( n != nullptr )
    {
        *n = n_tabs;
    }

    if ( n_tabs > 0 )
    {
        for ( int i = 0; i < n_tabs; i++ )
        {
            tmp += "\t";
        }
    }
    return tmp;
}

