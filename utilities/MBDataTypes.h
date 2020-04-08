// -*- mode: c++ -*-

/********************************************
* Author: Per Thomas Hille <pth@embc.no> ****
*********************************************/

/****************************************************
* Copyright: Embedded Consulting AS and others.
* For more information please contact pth@embc.no
******************************************************/

#pragma once

//#include <common/KGDataType.h>


#include <utilities/GDefinitions.h>

#include <ostream>
using std::ostream;

#include <string>

using std::string;

template<typename T>
class MBDataType
{ 
public: 
	///inline explicit   MBDataType() : fVal( T() )  { }; 
	inline explicit   MBDataType(const T val) : fVal(val) { };
	inline virtual   ~MBDataType() { };
	inline T GetValue() const { return fVal; };
	inline void SetValue( const T val) { fVal = val; };
	inline string str() const 
	{ 
		std::stringstream buffer; 
		buffer << fVal; 
		return buffer.str(); 
	}; 
	inline const char * c_str() const 
	{ 
		static string tmp; 
		tmp =  str(); 
		return tmp.c_str(); 
	}; 
	inline virtual  MBDataType & operator = (const T &rhs) 
	{ 
		SetValue(rhs); 
		return *this; 
	}; 
private: 
	T fVal; 
};  


template<typename T>
inline bool operator  == (const MBDataType<T>  &lhs, const MBDataType<T>  &rhs)
{ 
	return lhs.GetValue() == rhs.GetValue() ? true : false; 
} 

template<typename T>
inline bool operator  == (const MBDataType<T> &lhs, const T &rhs) 
{ 
	return lhs.GetValue() == rhs ? true : false; 
} 
 
 template<typename T>
inline bool operator  == (const T &lhs, const MBDataType<T> &rhs)
{ 
	return  lhs == rhs.GetValue() ? true : false; 
} 


template<typename T>
inline bool operator  != (const MBDataType<T> &lhs, const MBDataType<T>  &rhs)
{ 
	if (lhs.GetValue() == rhs.GetValue())
	{ 
		return false; 
	} 
	else 
	{ 
		return true; 
	} 
}
 
template<typename T> 
inline ostream& operator << (ostream& os, const MBDataType<T> &in) 
{ 
	std::stringstream buffer; 
	os << in.GetValue(); 
	return os; 
} 


class MBComponentID_t : public MBDataType<string> 
{ 
	public:
		MBComponentID_t( ) : MBDataType("") {};
		MBComponentID_t(string val) : MBDataType(val) {};
};

 

class MBSubscriberChannel_t : public MBDataType<string> 
{ 
	public:
		MBSubscriberChannel_t( ) : MBDataType("") {};
		MBSubscriberChannel_t(string val) : MBDataType(val) {};
};


class MBRequestChannel_t : public MBDataType<string> 
{ 	public:
		MBRequestChannel_t( ) : MBDataType("") {};
		MBRequestChannel_t(string val) : MBDataType(val) {};
};


class MBReplyChannel_t : public MBDataType<string> 
{ 
	public:
		MBReplyChannel_t( ) : MBDataType("") {};
		MBReplyChannel_t(string val) : MBDataType(val) {};
};


class MBTime_t : public MBDataType<int> 
{ 
	public:
		MBTime_t( ) : MBDataType(0) {};
		MBTime_t(int val) : MBDataType(val) {};
};




// GENERATE_DATATYPE(MBComponentID_t, string);
// GENERATE_DATATYPE(MBSubscriberChannel_t, string);
// GENERATE_DATATYPE(MBRequestChannel_t, string);
// GENERATE_DATATYPE(MBReplyChannel_t, string);
// GENERATE_DATATYPE(MBTime_t, uint64_t);


inline bool operator  < (const MBSubscriberChannel_t &lhs, const MBSubscriberChannel_t &rhs) 
{ 
	return lhs.str() < rhs.str() ? true : false;
} 
 