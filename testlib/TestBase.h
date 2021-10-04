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

#ifdef HAS_LOGGING
extern int argc_;
extern char **argv_;
#include <cmdline/GLogApplication.h>
#include <cmdline/GCmdScan.h>
#include <utilities/GFileIOHandler.h>
#include <logging/LLogApi.h> 
using namespace LOGMASTER;
#endif

#include <string>
using std::string;

#ifdef _WIN32
#include <gtest/gtest.h>
#else
#include <gtest-embc/gtest.h>
#endif

#define CATCH_GTEST_EXEPTION                                  \
	catch (GException & e)                                    \
	{                                                         \
		CERR << "WHAT1!!:" << e.what() <<  endl;                             \
		FAIL();                                               \
	}                                                         \
	catch (testing::internal::GoogleTestFailureException & e) \
	{                                                         \
		CERR << "WHAT2!!: "<< e.what() << endl;                             \
		FAIL();                                               \
	}                                                         \
	catch (const std::exception &e)                           \
	{                                                         \
		CERR << "WHAT3!!:" << e.what() << '\n';                             \
		FAIL();                                               \
	}                                                         \
	catch (std::string & e)                                 \
	{                                                         \
		CERR << "WHAT4!!:" << e << endl;                                    \
		FAIL();                                               \
	}                                                         \
	catch (...)                                               \
	{                                                         \
		CERR << "WHAT5!!: Unknown excpeption caught" << endl;          \
		FAIL();                                               \
	}


/** @brief Base class for all unit tests */
#ifndef HAS_LOGGING
class TestBase : public testing::Test
{
};
#else
class TestBase : public testing::Test
{

public:
	API inline TestBase();
	API virtual inline ~TestBase();
	API string FileIOTest(const string fname = "");

protected:
	LLogging *l = LLogging::Instance();
	string fOldLogFileName = "not_set.log";
	string fTestLogFileName = "googletest.log";
	GLogApplication *g = nullptr;

private:
	inline void API Init();
	inline TestBase(TestBase &);
	inline void operator=(TestBase &);
};

inline TestBase::TestBase()
{
	SET_LOGTARGET("--target-off --target-file");
        //g = new GLogApplication(argc_, (const char **)argv_, nullptr, true);
        g = new GLogApplication(1, (const char **)argv_, nullptr, true); // Temp fix, see ESCORE-1327
        Init();
}

inline TestBase::~TestBase()
{
	//l->Pop();
}

inline void
TestBase::Init()
{
	fOldLogFileName = LLogging::Instance()->GetLogFileName(eMSGTARGET::TARGET_FILE);
	SET_LOGFILENAME(fTestLogFileName);
	g_cmdscan()->SetIgnoreStrayArgument(true);
	GException::DisableStackTrace();
}

/*Here we check the last line written to the log file*/
inline string
TestBase::FileIOTest(const string fname)
{
	string fname_local = fname == "" ? l->GetLogFileName(eMSGTARGET::TARGET_FILE) : fname;
	string last = g_file()->ReadLastLine(fname_local, 1);
	return last;
}

#endif
