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


#include "GNumbers.h"



GNumbers * g_numbers()
{
	static GNumbers *instance = new GNumbers();
	return instance;
}


void
GNumbers::DisableError()
{
    fIsDisabledError = true;
}


void
GNumbers::EnableError()
{
    fIsDisabledError = false;
}


bool
GNumbers::IsFloatTypeS(string type)
{
    const string t = string(type);

    if (t == typeid(float).name() ||
        t == typeid(double).name() ||
        t == typeid(long double).name())
    {
        return true;
    }
    else
    {
        return false;
    }
}


/**
* Converting a number on hexadecimal form represented as a string into
* the same number writen on decimale base 10 format (also represented as a string)
* @param str Input string on hex format. NB it must start with either 0x or 0X succeded
* witk a valid sequence of hex digits (0-F) othervise and exeption will be thrown.
* @return The input number on decinmal base 10 format. */
 string
GNumbers::Hex2Dec(const string  str)
{
    std::stringstream buffer;
    long long int     num = ToHex(str);
    buffer << num;
    return buffer.str();
}


/**
* Converting a number on decimal from (base 10) form represented as a string into
* the same number writen on hexadecimal format (also represented as a string)
* @param str Input string on DEC format.
* with a valid sequence of hex digits (0-F) othervise and exeption will be thrown.
* @return The input number on hexadecimal form. */
string
GNumbers::Dec2Hex(const string  str)
{
	std::stringstream buffer;
    if (IsHex(str) == true)
    {
        string message = str + " is not a decimal number, it is a HEX number, but this function converts from dec to hex";
		GCommon().HandleError ( message, GLOCATION, IsDisabledError());
		return "";
	}
	else
	{
		long long int     num = ToInteger<long long int>(str);
		buffer << std::hex << "0x" << num;
	}
	return buffer.str();
}


 bool
GNumbers::IsAlphaNumber(string num)
{
    num = g_string()->Trim(num, {' ', '\t', '\n'});

    for (uint16_t i = 0; i < num.size(); i++)
    {
        int val = (int)num[i];
        if (val >= 0 && val <= 255)
        {
            if (!isalnum(num[i]))
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}


 bool
GNumbers::IsBinary(string num)
{
    g_string()->Trim(num);
    g_string()->Ltrim(num, '-');
    for (uint16_t i = 0; i < num.size(); i++)
    {
        if (num[i] != '1' && num[i] != '0')
        {
            return false;
        }
    }
    return true;
}


 bool
GNumbers::IsDecNumber(const string  num)
{
    if (IsDigit(num) || IsFloat(num))
    {
        if (IsHex(num))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}


/* Wether or not the string "num" is digit in base given by "base". For base 10
*  The digits shall be 0-9, for base 16 0-F  etc..
*  @param num  The string represenation of the digit
*  @param base the base, or radix of the number "num"
*  @return  true if the number given by "num" is a valid digit in the base "base", false othervise */
 bool
GNumbers::IsDigit(const string num, const int base)
{
    string tmp = num;
    g_string()->Trim(tmp, ' ');

    if (tmp == "0")
    {
        return true;
    }
    else
    {
        g_string()->Ltrim(tmp, '0');
    }
    if (tmp.size() != 1) return false;

    bool iret = false;
    try
    {
        return iret = stoul(tmp, 0, base) == 0 ? false : true;
    }
    catch (...)
    {
        return false;
    }
//    return iret;
}


 bool
GNumbers::IsHex(const string in)
{
    string s = in;
    g_string()->Trim(s);
    g_string()->Ltrim(s, '-');
   if (  ( g_string()->BeginsWith(s, "0x", true) )  &&  ( (s.size() >  2 ))  )
    {
        for (uint16_t i = 2; i < s.size(); i++)
        {

            if (isxdigit(s[i]) == false)
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
    return false;
}


 bool
GNumbers::IsInteger(const string num)
{
    string tmp = num;
    g_string()->Trim(tmp, {' ', '\t', '\n'});

    if (IsFloat(tmp) == true)
    {
        return   std::stold(tmp.c_str()) - std::atoll(tmp.c_str()) == 0 ? true : false;
    }
    return false;
}


bool
GNumbers::IsNumber(const double num)
{
    return IsNumber(g_string()->ToString(num));
}






bool
GNumbers::IsNumber(const string  num)
{
    string trimmed = num;
    g_string()->Trim(trimmed, { ' ', '\t', '\n' });

    if (IsHex(trimmed) || IsBinary(trimmed) || IsDigit(trimmed) || IsFloat(trimmed) || IsAlphaNumber(trimmed) )
    {
        return true;
    }
    else
    {
        return false;
    }
}





 bool
GNumbers::IsFundamentalTypeS( string type )
{
    if (string(type) == typeid(bool).name() ||
        IsFloatTypeS( string(type) ) ||
        IsIntegerTypeS( string(type)  ))
    {
        return true;
    }
    else
    {
        return false;
    }
}


 bool
GNumbers::IsUnsignedTypeS(string type)
{
    const string  t = string(type);
    if (
        string(type) == typeid(unsigned char).name() ||
        t == typeid(unsigned short).name() ||
        t == typeid(unsigned int).name() ||
        t == typeid(unsigned long int).name() ||
        t == typeid(unsigned long long int).name())
    {
        return true;
    }
    else
    {
        return false;
    }
}


 bool
GNumbers::IsFundamentalVTypeS(string type)
{
    if (type == typeid(vector<bool>).name() ||
        IsFloatVTypeS(type) ||
        IsIntegerVTypeS(type))
    {
        return true;
    }
    else
    {
        return false;
    }

}


 bool
GNumbers::IsFloatVTypeS(string type)
{
    //    string type = typeid(T).name();
    if (type == typeid(vector<float>).name() ||
        type == typeid(vector<double>).name() ||
        type == typeid(vector<long double>).name())
    {
        return true;
    }
    else
    {
        return false;
    }
}



 bool
GNumbers::IsIntegerVTypeS(string type)
{
    if (type == typeid(vector<char>).name() ||
        type == typeid(vector<short>).name() ||
        type == typeid(vector<int>).name() ||
        type == typeid(vector<long int>).name() ||
        type == typeid(vector<long long int>).name() ||
        type == typeid(vector<unsigned char>).name() ||
        type == typeid(vector<unsigned short>).name() ||
        type == typeid(vector<unsigned int>).name() ||
        type == typeid(vector<unsigned long int>).name() ||
        type == typeid(vector<unsigned long long int>).name())
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}


 bool
GNumbers::IsIntegerTypeS(string t)
{
	int cnt = 0;

	if (t == string(typeid(short).name()) ||
		t == string(typeid(int).name()) ||
		t == string(typeid(long int).name()) ||
		t == string(typeid(long long int).name()) || IsUnsignedTypeS(t))
	{
		cnt++;


		return true;
	}
	else
	{
		cnt++;

		return false;
	}
}


 long long int
GNumbers::ToHex(const string num)
{
	string s = num;
	g_string()->Trim(s);
	if (IsHex(s))
	{
		return stoull(s, 0, 16);
	}
	else
	{
		string message = s + " is NOT a valid hex number string, please make sure that the number starts with 0x folowed by valid hex digits(0 - F)";
		GCommon().HandleError(message, GLOCATION, IsDisabledError() );
	}
	return -99999; // Never reached, but just in case
}


/* @brief Converts a binary number string to a 64 bits integer. The string is intepretet assuming radix 2 (i.e binary)
*  @param[in] b  The string to convert
*  @return the corresponding number as a 64 bit int
*  @exception GException  if the system dependent maximum  number if bits is exceede, or if the string has wrong format. Ths is,
*  not valid binary string format containg 0'oes nad 1'nes, and/or an optional precedding minus sign. */
 int64_t
GNumbers::ToBinary(const string b)
{
    string s = b;
    g_string()->Trim(s);
    bool negative = g_string()->BeginsWith(s, "-", false);
    int64_t tmp = 0;
    int64_t maxbits = sizeof(long long) * 8;
    int64_t BitWidths = BitWidth(s);

    if (BitWidths > maxbits)
    {
        char message[512];
        #ifdef ARM
        snprintf(message, 512,"Bitstream constains %lld bits, ( bitstring = %s ). The max number of bits is: %lld", BitWidths, b.c_str(), maxbits);
#endif
		


		//   #else
#ifdef _WIN32
		SPRINTF(message, 512,"Bitstream constains %lld bits, ( bitstring = %s ). The max number of bits is: %lld", BitWidths, b.c_str(), maxbits);
#else
#ifdef ARM

        SPRINTF(message, 512, "Bitstream constains %lld bits, ( bitstring = %s ). The max number of bits is: %lld", BitWidths, b.c_str(), maxbits);
#else
        SPRINTF(message, 512, "Bitstream constains %ld bits, ( bitstring = %s ). The max number of bits is: %ld", BitWidths, b.c_str(), maxbits);

#endif

#endif

// #ifndef G_STANDALONE
        GCommon().HandleError(message, GLOCATION, IsDisabledError() );
// #endif
    }

    if (IsBinary(s) == true)
    {
        size_t n = s.size();
        for (size_t i = 0; i < n; i++)
        {
            if (s[n - i - 1] == '1')
            {
#ifdef _WIN32
                tmp = tmp | (1i64 << i);
#else
                tmp = tmp | (1 << i);
#endif
            }
        }
    }
    else
    {
        string message = s + "\t is not a binary number string, the string must contain only ZERO and ONES prefixed by an optional - (minus) sign";
// #ifndef G_STANDALONE
       GCommon().HandleError( message, GLOCATION, IsDisabledError() );
// #endif
    }
    return negative == true ?  -tmp : tmp;
}


/**@{
* Evaluates the the witdt of the binary number "in" repsented on string format in number of bits.
* For xample "0010101" is 5 bits wide (discarding preceeeding zeroes), "111" is 3 bits wide, etc
* @param[in] in Must be a binary number, i.e  a string containig only "0" and "1".
* @exception std::exception if the string "in" is not a valid binary number
* @return The widt in number of bits */
int64_t
GNumbers::BitWidth(const char *in)
{
    return BitWidth(string(in));
}

int64_t
GNumbers::BitWidth(const string in)
{
    int64_t npos = 0;

    if (GNumbers::IsBinary(in) == false)
    {
        string message = in + "%is not a valid binary number: The string must contain only zeroes and ones, and start with a b";
		GCommon().HandleError(message, GLOCATION, IsDisabledError() );
		return -1;
    }
    else
    {
        npos = in.size() - in.find_first_of('1');
        return npos;
    }
}
/**@}*/



bool 
GNumbers::IsFloat(const char * num)
{
     return IsFloat(string(num));
}



bool
GNumbers::IsFloat(string num)
{
    num = g_string()->Trim(num, { ' ', '\t', '\n' });

	// Exceptions is a pain, so lets remove some common causes.
    if (num == ",")
    {
        return false;
    }
    if (g_string()->BeginsWith(num, "--"))
    {
        return false;
    }
    if ((num.size() == 3) && (::tolower(num.at(0)) == 'n') && (::tolower(num.at(1)) == 'a') && (::tolower(num.at(2)) == 'n'))
    {
        return true;
    }
    const char* p = num.c_str();
    while (*p)
    {
        if ((*p == 'E') || (*p == 'e') || (*p == '.') || (*p == ',') || (*p == '-') || (*p == '+') || (*p >= '0' && *p <= '9'))
        {
            p++;
        }
        else
        {
            return false;
        }
    }
    
    try
    {
        auto t = std::stold(num);

    }
    catch ( ... )
    {
        return false;
    }

    return true;
   
}



