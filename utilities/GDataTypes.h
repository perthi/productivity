// -*- mode: c++ -*-
#ifndef GDATATYPES_H
#define GDATATYPES_H

/****************************************************************************
*** Copyright(C) 2015  Per Thomas Hille, pth@embc.no                     ***
*** This file is part of logmaster. logmaster is free software : you can ***
*** redistribute it and / or modify it under the terms of the Lesser GNU ***
*** General Public License (LGPL) V3 or later. See .cpp file for details ***
*****************************************************************************/


#include "GDefinitions.h"
#include "GCommon.h"
#include "GText.h"
#include "GLocation.h"

#include <ostream>
using std::ostream;
#include <sstream>
#include <typeinfo>

#ifndef _WIN32
#include <execinfo.h>
#endif

#include <iostream>

using std::endl;
using std::cout;

class Val
{
public:
    Val(const double t, const double min, 
        const double max, const string name, 
        const string subscript, const string helpGText): fVal(t), 
                                                         fMinValue(min), 
                                                         fMaxValue(max), 
                                                         fTypeId("NOT SET"),  
                                                         fName(name),
                                                         fSubscript(subscript),
                                                         fHelpText(helpGText)
    { 
	
    }
    
    virtual ~Val() {}; 
    
    
    virtual double GetValue( ) const
    {
        return fVal;
    }
    

    virtual void API SetValue(const double  value);
    virtual void API CheckLimits(const double &t, const double min, const double max);
    
	
    void GeneratStackFrames();
    void CheckIsInteger(double t);
    virtual  Val & operator = (const double rhs) = 0;
  
protected:
    
    double fVal;
    double fMinValue;
    double fMaxValue;
   
    string fTypeId;
    string fName;
    string fSubscript;
    string fHelpText;
};



template <class T>
class Val_t : public Val
{
public:
    Val_t(T  t, T min, T max, const string name = "", const string subscript = "", const string helpGText = "");
    virtual ~Val_t() {};
    string GetName() const  {return fName; }
    const char *  GetValueCString();
    string GetSubscript() const { return fSubscript; }
    string GetHelpText() const { return fHelpText;}
    double GetMin() const { return fMinValue; }
    double GetMax() const { return fMaxValue; }
    void   SetName( const string name ){ fName = name;}
    void   SetHelpText( const string helpGText ) { fHelpText = helpGText; }
    void   SetSubscript( const string sub){ fSubscript = sub;}
    virtual string GetValueString();
    virtual void Set(const T value, const T min, const T max );
    virtual void SetLimits(const T min, const T max);
    void Print();
    void PrintParameter( bool details = false );
    void PrintParameterUsage();
    virtual void PrintDefaults(); 
    virtual  Val_t & operator = (const double rhs);
};



template <class T>
Val_t<T>::Val_t(T  t, T min, T max, const string name,
                const string subscript, const string helpGText) : Val(t, min, max, name, subscript, helpGText)
{
    fTypeId = typeid(T).name();
    CheckLimits(t, min, max);
}


template <class T>
const char *  
Val_t<T>::GetValueCString()
{
    static char  tmp[512];
    snprintf(tmp, 512, "%s", GetValueString().c_str());
    return tmp;
}



template <class T>
string 
Val_t<T>::GetValueString()
{
    std::stringstream ss;
    ss << fVal;
    return ss.str();
}
  

template <class T>
void 
Val_t<T>::Set(const T value, const T min, const T max )
{
    SetLimits(min, max);
    SetValue(value);
    CheckLimits( value, min, max );
}



template <class T>
void Val_t<T>::SetLimits(const T min, const T max)
{
    if (max < min)
    {
        std::stringstream buffer;
        buffer << "min cannot be bigger than max, you tried to set  min=" << min << " and max = " << max << ",.. aborting" << endl;
        GCommon().HandleError(  GText( "%s", buffer.str().c_str()  ).str() , GLOCATION, THROW_EXCEPTION ) ;
    }
    else
    {
        fMinValue = min;
        fMaxValue = max;
    }
}


template <class T>
void 
Val_t<T>::Print()
{
    cout << "*******************" << endl;
    cout << "Name:\t"  << fName << "*****" << endl;
    cout << "Min:\t"   << fMinValue << endl;
    cout << "Max:\t"   << fMaxValue << endl;
    cout << "Value:\t" << fVal << endl;
    cout << "*******************" << endl;
}


template <class T>
void 
Val_t<T>::PrintParameterUsage()
{
    cout << "\t\t\t" << fName << ":" << (fName.size() < 6 ? "\t" : "") << "\t(" << fSubscript << ")"
         << "\t" << fHelpText;
    cout << endl
         << "\t\t\t" << (fName.size() < 6 ? "\t" : "");
    PrintDefaults();
}


template <class T>
void
Val_t<T>::PrintDefaults()
{
    printf("\t\tdefault=%0.2G, [min, max] = [%0.2G,%0.2G]\n", (double)fVal, fMinValue, fMaxValue  );	
}


/*********************************************************
 ****************** OPERATORS ****************************
 ********************************************************/
ostream& operator<<(ostream& os, const Val  &o);

template <class T>
Val_t<T> & Val_t<T>::operator = (const double rhs)
{
    SetValue(rhs);
    return *this;
}


template<class T>  
ostream& operator<<(ostream& os, Val_t<T>  &o)
{
    os << o.GetValue(  ) << o.GetSubscript();   
    return os;
} 



template <class T>
inline bool operator  == (const Val_t<T> &lhs, const Val_t<T> &rhs)
{
    if (lhs.GetValue() == rhs.GetValue())
    {
        return true;
    }
    else
    {
        return false;
    }
}



/*
  template <class T>
  inline void operator  = (const Val_t<T> &lhs, const Val_t<T> &rhs)
  {
   
  }
*/


template <class T>
inline Val_t<T> operator  + ( const Val_t<T> &lhs,  const Val_t<T> &rhs)
{
    Val_t<T> tmp = lhs;
    if ( ! ( lhs.GetName() == rhs.GetName() )  )
    {
       GCommon().HandleError(GText(   "You cannot add to variables of different types, you have tried to add %s (%s = %f) and %s ( %s = %f) which is forbidden", 
                  lhs.GetName().c_str(), lhs.GetSubscript().c_str(), 
                  lhs.GetValue(), rhs.GetName().c_str(), rhs.GetSubscript().c_str(), rhs.GetValue() ).str(), GLOCATION, THROW_EXCEPTION  ); 

    }
    else
    {
        double val1 = lhs.GetValue();
        double val2 = rhs.GetValue();
        tmp.SetValue(val1 + val2);
    }
    return tmp;
}


template <typename T, typename T2>
inline double operator  + (double &lhs, const Val_t<T2> &rhs)
{
    return  lhs + (double)rhs.GetValue();
}


template <class T>  
inline double operator  + (  const Val_t<T> &lhs,   const double &rhs )
{
    return  (double)lhs.GetValue() +  (double)rhs;
}



template <class T1, class T2>
inline Val_t<T1> operator  - (const Val_t<T1> &lhs, const Val_t<T2> &rhs)
{
    Val_t<T1> tmp = lhs;

    if (!(lhs.GetName() == rhs.GetName()))
    {
         GCommon().HandleError(GText(   "You cannot subtract to variables of different types, you have tried to subtract   %s (%s = %f) and %s ( %s = %f) which is forbidden", 
                                          lhs.GetName().c_str(), lhs.GetSubscript().c_str(), lhs.GetValue(), 
                                          rhs.GetName().c_str(), rhs.GetSubscript().c_str(), rhs.GetValue() ).str(), GLOCATION, THROW_EXCEPTION ); 
    }
    else
    {
        double val1 = lhs.GetValue();
        double val2 = rhs.GetValue();
        tmp.SetValue(val1 - val2);
    }
    return tmp;
}



//template <typename T = std::enable_if_t < std::is_fundamental < T > ::value>, typename T2 >
template <typename T2> 
inline double operator  - (double lhs, const Val_t<T2> &rhs)
{
    return  lhs - (double)rhs.GetValue();
}



template <class T>
inline double operator  - (const Val_t<T> &lhs, const double &rhs)
{
    return  (double)lhs.GetValue() - (double)rhs;
}



template <class T, class T2>  
bool  operator <  ( const T &lhs, const  Val_t<T2> & rhs) 
{
    if( lhs < rhs.GetValue() ) { return true; }
    else { return false;}
}


template <class T, class T2>  
inline double operator *  ( const Val_t<T> &lhs, const Val_t<T2> & rhs) 
{
    return (double)lhs.GetValue()*(double)rhs.GetValue();
}


template <class T>  
inline double operator *  (int lhs, const Val_t<T> & rhs) 
{
    return (double)lhs*(double)rhs.GetValue();
}


template <class T, class T2>
inline double operator /  (const Val_t<T> &lhs, const Val_t<T2> & rhs)
{
    if (rhs.GetValue() == 0)
    {
        GCommon().HandleError(GText(   "Attempt to divide by ZERO !  %f / %f = inf", lhs.GetValue(), rhs.GetValue()).str(),   GLOCATION, THROW_EXCEPTION  ); 
    }


    return (double)lhs.GetValue() / (double)rhs.GetValue();
}


template <class T>
inline double operator /  (int lhs, const Val_t<T> & rhs)
{
    if (rhs.GetValue() == 0)
    {
        GCommon().HandleError(GText(   "Attempt to divide by ZERO !  %d / %f = inf", lhs, rhs.GetValue()).str() , GLOCATION, THROW_EXCEPTION  ); 
    }
    return (double)lhs / (double)rhs.GetValue();
}



template <class T1, class T2, class T3 >
void setLimits(T1 &v, T2 min, T3 max)
{
    for (unsigned int i = 0; i < v.size(); i++)
    {
        v[i].SetLimits(min, max);
    }
}



#endif
