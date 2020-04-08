


#include "TestException.h"
///#include <exception/GException.h>
#include <testlib/TestBase.h>
#include <utilities/GText.h>
#include <utilities/GRandom.h>
#include <utilities/GFileIOHandler.h>
#include <utilities/GString.h>
//#include <logging/LMessageGenerator.h>



TestException::TestException() : TestBase()
{

}


TestException::~TestException()
{

}


void 
TestException::SetUp()
{
    TestBase::SetUp();
}



TEST_F( TestException, simple)
{
    EXPECT_ANY_THROW(EXCEPTION( "a simple exeption" ) );
	
	EXPECT_THROW(EXCEPTION("a simple exeption"), GException);
    EXPECT_THROW( RANGE_EXCEPTION("a simple exeption"), GRangeException );
    EXPECT_THROW(FILE_NOT_FOUND_EXCEPTION("a simple exeption"), GFileNotFoundException );
}





/// Here we check that the expected mesaage is written to the logfile by the logging system 
TEST_F(TestException, fileIO)
{
    string f1 = g_random()->Name("exception_fileio_test", ".txt");
    string f2 = g_random()->Name("exception_fileio_test", ".txt");

    SET_LOGFILENAME(f1);
    SET_LOGTARGET("0000 --target-file");
    SET_LOGFORMAT("01000001");

    EXPECT_ANY_THROW(EXCEPTION("a simple exception"));
    EXPECT_EQ(g_file()->ReadFirstLine(f1),  "<Error:Exeption>         \ta simple exception (class GException)");
    
    SET_LOGFILENAME(f2);

    string f = "dontexist.txt";
    EXPECT_THROW(FILE_NOT_FOUND_EXCEPTION("Cannot find file: %s", f.c_str()), GFileNotFoundException);
    EXPECT_EQ( g_file()->ReadFirstLine(f2), "<Error:Exeption>         \tCannot find file: dontexist.txt (class GFileNotFoundException)");
    
    g_file()->Delete(f1);
    g_file()->Delete(f2);
 }
 




TEST_F(TestException, NSR248)
{
    SET_LOGFORMAT("00000001");
//	SET_LOGLEVEL("--debug");
//	SET_LOGTARGET("--target-exception");

    try
    {
        throw(GText("A Simple GText with a parameter = %d", 42));
    }
    catch(GText &e)
    {
        EXPECT_EQ(true,  g_string()->BeginsWith(e.what(), "A Simple GText with a parameter = 42", false ));
    }
    try
    {
        EXCEPTION("A Simple Text with another parameter = %d", 69);
    }
    catch (GException &e)
    {
       EXPECT_EQ(true,  g_string()->Contains(e.what(), "Simple Text with another parameter = 69 (class GException)", false));
       EXPECT_STREQ( "\tA Simple Text with another parameter = 69 (class GException)\n", e.what() );
    }     
}



TEST_F(TestException, assert_macro)
{
	EXPECT_THROW(G_ASSERT_EXCEPTION(false, "Testing the assert macro"), GException);
	EXPECT_NO_THROW(G_ASSERT_EXCEPTION(true, "Testing the assert macro"));

	EXPECT_THROW(FSM_ASSERT_EXCEPTION(false, "Testing the FSM assert macro"), GFSMException);
	EXPECT_NO_THROW(FSM_ASSERT_EXCEPTION(true, "Testing the FSM assert macro"));

//	EXPECT_THROW(DRIVER_ASSERT(false, "Testing the DRIVER assert macro"), GDriverException);
//	EXPECT_NO_THROW(DRIVER_ASSERT(true, "Testing the DRIVER assert macro"));

//	EXPECT_THROW(DB_ASSERT(false, "Testing the DATABASE assert macro"), GDatabaseException);
//	EXPECT_NO_THROW(DB_ASSERT(true, "Testing the DATABASE assert macro"));
}




TEST_F(TestException, buffer_overwrite_NSR1737)
{
	PUSH();
	SET_LOGFORMAT("00000001");
	SET_LOGTARGET("0000 --target-file");
	
	 char *msg = 0;

	try
	{
		EXCEPTION("lorem ipsum");
	}
	catch(GException &e)
	{
		msg = G_ERROR( "The exception message is:%s",  e.what( ) )->at(eMSGTARGET::TARGET_EXCEPTION)->fMsgBody;
	}
			
	EXPECT_STREQ("The exception message is:\tlorem ipsum (class GException)\n", msg );
	POP();
}




TEST_F(TestException, disable_execptionNSR1769)
{
	EXPECT_ANY_THROW(EXCEPTION("an exception"));
	EXPECT_ANY_THROW(RANGE_EXCEPTION("an exception"));
	EXPECT_ANY_THROW(FILE_NOT_FOUND_EXCEPTION("an exception"));
	EXPECT_ANY_THROW(INVALID_ARGUMENT_EXCEPTION("an exception"));
	EXPECT_ANY_THROW(MISSING_ARGUMENT_EXCEPTION("an exception"));
	EXPECT_ANY_THROW(FSM_EXCEPTION("an exception"));	

	EXPECT_ANY_THROW(G_ASSERT_EXCEPTION( false, "an exception"));
	EXPECT_ANY_THROW(FSM_ASSERT_EXCEPTION(false, "an exception"));
	
	EXPECT_NO_THROW(G_ASSERT_EXCEPTION(true, "an exception"));
	EXPECT_NO_THROW(FSM_ASSERT_EXCEPTION(true, "an exception"));

	EXPECT_TRUE(GException::IsEnabledException());

	GException::DisableException();
	EXPECT_FALSE(GException::IsEnabledException());
	GException::EnableException();
	EXPECT_TRUE(GException::IsEnabledException());
	GException::DisableException();

	EXPECT_NO_THROW(EXCEPTION("an exception"));
	EXPECT_NO_THROW(RANGE_EXCEPTION("an exception"));
	EXPECT_NO_THROW(FILE_NOT_FOUND_EXCEPTION("an exception"));
	EXPECT_NO_THROW(INVALID_ARGUMENT_EXCEPTION("an exception"));
	EXPECT_NO_THROW(MISSING_ARGUMENT_EXCEPTION("an exception"));
	EXPECT_NO_THROW(FSM_EXCEPTION("an exception"));
	EXPECT_NO_THROW(G_ASSERT_EXCEPTION(false, "an exception"));
	EXPECT_NO_THROW(FSM_ASSERT_EXCEPTION(false, "an exception"));
}



TEST_F(TestException, missing_messageNSR2366)
{
	GException::EnableException();
	int number = 42;
	SET_LOGFORMAT("00000001");
	
	try
	{
		EXCEPTION("lorem ipsum, the number is %d", 42);
	}
	catch (GException &e)
	{
		EXPECT_STREQ( "\tlorem ipsum, the number is 42 (class GException)\n", e.what() );
	}

	SET_LOGLEVEL("--all-off");

	try
	{
		EXCEPTION("lorem ipsum, the number is %d", 42);
	}
	catch (GException &e)
	{
		EXPECT_STREQ("\tlorem ipsum, the number is 42 (class GException)\n", e.what());
	}
}

