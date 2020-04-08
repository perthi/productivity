#pragma once
//#include "TestLogging.h"

#include <testlib/TestBase.h>


/* Unit tests for the exception handling system. All messages creted by the exception handling system 
 * is passed on to the logging system, and logged exactly the same way as a regular log message.
 * The TestException test fixture therefore inherits from the  TestLogging fixture. */


class TestException : public TestBase
   // public TestLogging
{
public:
    TestException();
    virtual ~TestException();
    virtual void SetUp();
};


