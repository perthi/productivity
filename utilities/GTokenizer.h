// -*- mode: c++ -*-

#ifndef GTOKENIZERXXX_H
#define GTOKENIZERXXX_H

/***************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                     ****
****************************************************************************/

/****************************************************************************
*** Copyright(C) 2018  Per Thomas Hille, pth@embc.no                      ***
*** This file is part of logmaster.logmaster is free software : you can   ***
*** redistribute it and / or modify it under the terms of the Lesser GNU  ***
*** General Public License(LGPL) V3 or later.See.cpp file for details     ***
*****************************************************************************/


/** @class GTokenizer
*   @brief Class for tokenizing strings based on an arbritray set of separators 
*
*   The class also tokenize C style command lines (e,g int argc const char **argv) */


#include "GConstants.h"
using namespace GCONSTANTS;

#include "GDefinitions.h"

#include <string>
#include <vector>


using std::string;
using std::vector;


#define DISCARD_EMPTY     false
#define KEEP_EMPTY        true
#define DISCAR_SEPARATOR  false
#define KEEP_SEPARATOR           true
#define KEEP_TRAILING_SEPARATOR    true
#define DISCAR_TRAILING_SEPARATOR  false



class GTokenizer;

GTokenizer * g_tokenizer();


class  TestGTokenizer_Tokenize_Test; 

/** Class for tokenizing strings + some utility functions */
class GTokenizer
{
    friend TestGTokenizer_Tokenize_Test;  /* Make all functions accessible for google test **/
    friend  GTokenizer * g_tokenizer();
public: 
    GTokenizer() {};
    virtual ~GTokenizer() {};
     void              API   StripPath(const string fin, string &dir, string &fout, const bool keep_trailing_slash = KEEP_TRAILING_SEPARATOR );  
     vector<string>    API   Tokenize(const string source,  const string sep = "\t",   const bool keep_empty = DISCARD_EMPTY,  const bool keep_sep = DISCAR_SEPARATOR );
     vector<string>    API   Tokenize(const string source,  const vector<string> sep,  const bool keep_empty = DISCARD_EMPTY,  const bool keep_sep = DISCAR_SEPARATOR );
     vector<string>    API   Tokenize(const int argc, const char** argv);
     vector<string>    API   TokenizeCommandline(const string line);
     void              API   TokenizeCommandline(const string line,  int *argc,  const char **argv, const int max_size);

private:
     vector<string>   API   Tokenize(const vector<string> &source, const string sep = "\t", const  bool keep_empty = DISCARD_EMPTY, const bool keep_sep = DISCAR_SEPARATOR);
};




#endif
