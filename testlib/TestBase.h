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


#include <utilities/GDefinitions.h>

#ifdef  HAS_LOGGING
extern  int argc_;
extern  char** argv_;
#include <cmdline/GLogApplication.h>
#include <cmdline/GCmdScan.h>
#include <utilities/GFileIOHandler.h>
#include <logging/LLogApi.h> ///@todo replace with forward declaration
using namespace LOGMASTER;
#endif


#include <string>
using std::string;

#ifdef _WIN32
#include <gtest/gtest.h>
#else
#include <gtest-linux/gtest.h>
#endif




/** @brief Base class for all unit tests */
#ifndef  HAS_LOGGING
class TestBase : public testing::Test
{
	


};
#else
class TestBase : public GLogApplication, public testing::Test
{

public:
    API             inline TestBase( ); 
    API virtual	    inline ~TestBase();
    API string	     FileIOTest(const string fname = "");

protected:
    LLogging             *l = LLogging::Instance();
    string	       fOldLogFileName = "not_set.log";
    string	       fTestLogFileName= "googletest.log";

 private: 
     inline void API Init();
     inline TestBase(TestBase &);
     inline void operator=(TestBase &);
};


inline 
TestBase::TestBase() : GLogApplication()
{
	SET_LOGTARGET("--target-off --target-file");
	GLogApplication((const int)argc_, (const char**)argv_, nullptr, true);
	Init();
};

inline
TestBase::~TestBase()
{
	//l->Pop();
}


inline void
TestBase::Init()
{
	//   l->Push(  );
	fOldLogFileName = LLogging::Instance()->GetLogFileName(eMSGTARGET::TARGET_FILE);
	SET_LOGFILENAME(fTestLogFileName);
	//SET_LOGTARGET("0000 --target-file");
	g_cmdscan()->SetIgnoreStrayArgument(false);
	GException::DisableStackTrace();
}


/*Here we check the last line written to the log file*/
inline  string
TestBase::FileIOTest(const string fname)
{
	string fname_local = fname == "" ? l->GetLogFileName(eMSGTARGET::TARGET_FILE) : fname;
	string last = g_file()->ReadLastLine(fname_local, 1);
	return last;
}

#endif
