// -*- mode: c++ -*-
//#pragma once

#ifndef GSYSTEM_HPP
#define GSYSTEM_HPP
/***************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                     ****
****************************************************************************/

/****************************************************************************
*** Copyright(C) 2018  Per Thomas Hille, pth@embc.no                      ***
*** This file is part of logmaster.logmaster is free software : you can   ***
*** redistribute it and / or modify it under the terms of the Lesser GNU  ***
*** General Public License(LGPL) V3 or later.See.cpp file for details     ***
*****************************************************************************/



#include <string>
#include <vector>
using std::vector;
using std::string;

#include "GDefinitions.h"

class GSystem;

GSystem * g_system();


/** @brief Simplified interface for various system call for Linux and Windows */
class GSystem
{
    friend GSystem *g_system();

public:
    string         API      pwd(const bool print = false);
    vector<string> API      ls(const string dir = ".");
    bool           API      mkdir(const string dirname);
    bool           API      mkfile(const string filepath);
    void           API      cp(string src, string dest);
    int            API      rm(const string filename);
    void           API      mv(const string src, const string dest);
    string         API      exec(const char *cmd);
    string         API      GetProcessID();
    string         API      getenv(const string var);

#ifdef _WIN32
    string GetCommandLineAll();
    string GetCommandLineArguments();
#endif

    char *GetExePath();
    char *GetExeDir();
    char *GetExeName();
    string GetHostName();

    bool API Exists(const string filepath);
    string API GetDirectory(const string filepath);

#ifdef _WIN32
    char API *GetHomeDir();
#endif

private:
    GSystem(){};
    ~GSystem(){};
};

#endif