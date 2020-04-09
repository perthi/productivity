// -*- mode: c++ -*-


/*****************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                       ****
******************************************************************************/

/*****************************************************************************
******************************************************************************
*** This file is part of logmaster.                                        ***
*** Copyright (C) Per Thomas Hille <pth@embc.no>  http:///www.embc.no      ***
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

#ifdef _WIN32
#include <windows.h>
#include <WinBase.h>

#include "DbgHelp.h"
#pragma comment(lib, "Dbghelp.lib")
#endif

#include "GStackTrace.h"
#include <unistd.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string.h>
using std::cerr;
using std::endl;
using std::ostringstream;


GStackTrace::GStackTrace( )
{

}


GStackTrace::~GStackTrace()
{

}


#ifdef _WIN32
string
GStackTrace::str()
{
    std::ostringstream buffer;
    unsigned short frames;
    SYMBOL_INFO *symbol;
    HANDLE process;
    typedef USHORT(WINAPI * CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID *, __out_opt PULONG);
    //  CaptureStackBackTraceType func = (CaptureStackBackTraceType)(GetProcAddress(LoadLibrary(L"kernel32.dll"), "RtlCaptureStackBackTrace"));
    CaptureStackBackTraceType func = (CaptureStackBackTraceType)(GetProcAddress(LoadLibrary(L"kernel32.dll"), "RtlCaptureStackBackTrace"));

    if (func == NULL)
    {
        return "no stacktrace";
    }

    const int kMaxCallers = 62;

    void *callers_stack[kMaxCallers];
    process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);
    frames = (func)(0, kMaxCallers, callers_stack, NULL);
    symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    const unsigned short MAX_CALLERS_SHOWN = 100;
    frames = frames < MAX_CALLERS_SHOWN ? frames : MAX_CALLERS_SHOWN;
    for (unsigned int i = 0; i < frames; i++)
    {
        SymFromAddr(process, (DWORD64)(callers_stack[i]), 0, symbol);
        buffer << "*** " << i << ": " << callers_stack[i] << " " << symbol->Name << " - 0x" << symbol->Address;
        if (i < (unsigned int)(frames - 1))
        {
            buffer << std::endl;
        }
    }

    free(symbol);
    return buffer.str();
}

#else
string
GStackTrace::str()
{
#define BT_BUF_SIZE 100

    int j = 0;
    int nptrs = 0;

    void *buffer[BT_BUF_SIZE];
    char **strings;

    nptrs = backtrace(buffer, BT_BUF_SIZE);

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL)
    {
        return string(strerror(errno));
        // perror("backtrace_symbols");
        // exit(EXIT_FAILURE);
    }

    std::ostringstream buffer_s;
    for (j = 0; j < nptrs; j++)
    {
        buffer_s << j << "::::" << string(strings[j]) << endl;
        // printf("%s\n", strings[j]);
    }

    free(strings);

    return buffer_s.str();
}

#endif






const  char * 
GStackTrace::c_str()
{
    return str().c_str();
}


