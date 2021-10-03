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


#include "GLocation.h"
#include "GDefinitions.h"
#include <stdio.h>
#include <string>
#include <vector>

using std::vector;
using std::string;


#ifdef _WIN32
#define PATH_MAXLENGTH  _MAX_PATH
#else
#define PATH_MAXLENGTH  4096
#endif


class GFileIOHandler;
class TestGFileIOHandler_CheckFileNSR305_Test;

GFileIOHandler* g_file();


/** @brief Utility class for basic file IO operations such as creating, reading, and deleting files*/
class GFileIOHandler
{
    friend  GFileIOHandler* g_file();
    friend  TestGFileIOHandler_CheckFileNSR305_Test;

public:

    //    static GFileIOHandler API * Instance();
    string           API  ReadConfigFile(int argc, const char** argv, const string path);
    bool             API  CheckFile(const string fname, const char* opt = "r");    // checking if file exists
    bool             API  DoExists(const string fname, const char* opt = "r");    // checking if file exists
    bool             API  CheckFileEx(const string fname, const char* opt = "r");    // checking if file exists
    bool             API  Append(const string fname, const char* fmt, ...);
    bool             API  Delete(const string fname);
    bool             API  CreateFileLocal(const string fname, const bool print_error= true );
    bool             API  CreateFolder(const string fname, const bool print_error= true );
    vector<string>   API  ReadAll(const string fname, bool* status = nullptr);
    string           API  ReadFirstLine(const string fname);
    string           API  ReadLastLine(const string fname, const unsigned int offset = 0);
    string           API  ReadEntireASCIIFile(const string fname);
    string           API  GetAbsolutePath(const string fname);
    string           API  GetExtention(const string fname);
    bool             API  Recreate(const string fname, const bool print_error= true);

    bool             API  DeleteAll(const string fname);
    void             API  CreateDirIfNeeded(const std::string& filename);
    FILE             API* OpenFile(const string fname, const string opt, const GLocation loc, const bool print_error = true );

#ifdef _WIN32
    string           API  Errno2String(const  errno_t code, const string fname, const string  opt);
#else
    string           API  Errno2String(const  error_t code, const string fname, const string  opt);
#endif

#ifdef _WIN32
    void         SetAttribute(const string fname, unsigned long attr);
    void         ClearAttribute(const string fname, unsigned long attr);
#endif
private:


    GFileIOHandler() {};

};





