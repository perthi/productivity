

#include "GFormatting.h"
#include "GText.h"


#ifdef HAS_LOGGING
#include "exception/GException.h"
#else
class GInvalidArgumentException;
class GMissingArgumentException;
#include <exception>
#endif
#include <iostream>
#include <sstream>

#include <cstdint>
namespace GFormatting
{



template<typename T>
void 
ThrowException( const char * msg,  const char *file, const char *func, const int line )
{   
    #ifndef HAS_LOGGING
    throw(std::invalid_argument( GText("%s\nFormatting error at: %s::%s[line %d]", msg, file, func, line ).c_str()));
    #else
     throw_exception( T(  __FILE__,  __func__, __LINE__ , eMSGSYSTEM::SYS_EX, "%s:%s", msg, 
     GText("\nFormatting error at: %s::%s[line %d]",   file, func, line ).c_str()  ) );
//     MISSING_ARGUMENT_EXCEPTION(msg);
    #endif
}



bool actuallyCheckFormat( const char *file, int line, const char *func, const char *fmt, const Arg *args, const size_t numArgs)
{
    // here we can access arguments by index
    size_t currentArg = 0;

    while(*fmt != '\0')
    {
        if(*fmt == '%')
        {
            bool longSpecified = false;
            // Passing reference to self to have a recursive lambda function
            auto findSpecifier = [&currentArg, &fmt, args, numArgs,  file, line, func, &longSpecified](auto&& findSpec)
            {
                if(currentArg >= numArgs)
                {
                  
                     ThrowException<GMissingArgumentException>(   GText("Too few arguments, got %d, expected at least %d", 
                     numArgs, numArgs+1 ).c_str(),  file, func, line  ); 
                    
                    return false;
                }
                ++fmt;
                auto currChar = *fmt;
                if(longSpecified)
                {
                    switch(currChar)
                    {
                    case 'l':
                        return findSpec(findSpec);
                    case 'd':
                    case 'i':
                    case 'u':
                    case 'o':
                    case 'x':
                    case 'X':
                    if(args[currentArg].type != Arg::Type::INT)
                    {
                       ThrowException<GInvalidArgumentException> ( GText("Expected integer argument as argument %d", currentArg+1 ).c_str(),  
                        file,  func, line );
                        return false;
                    }
                    longSpecified = false;
                    ++currentArg;
                    return true;
                    default:
                        ThrowException<GInvalidArgumentException> ( GText( "Invalid format specifier at position %d", currentArg+1).c_str(),  
                         file, func, line   );
                        return false;
                    }

                }
                switch(currChar)
                {
                case '\0':
                    // Too few arguments
                   ThrowException<GMissingArgumentException>( GText( "Too few arguments, got %d, expected at least %d", 
                   numArgs, numArgs+1 ).c_str(), file, func, line );
                    return false;
                case '.':
                case '-':
                case '+':
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    // Look further
                    return findSpec(findSpec);
                case 'd':
                case 'i':
                case 'u':
                case 'o':
                case 'x':
                case 'X':
                    if(args[currentArg].type != Arg::Type::INT)
                    {
                        ThrowException<GInvalidArgumentException>  ( GText( "Expected integer argument as argument %d  )", 
                        currentArg+1 ).c_str(),  file, func, line );
                        return false;
                    }
                    ++currentArg;
                    return true;
                case 'l':
                    longSpecified = true;
                    return findSpec(findSpec);
                case 'f':
                case 'F':
                case 'e':
                case 'E':
                case 'g':
                case 'G':
                case 'a':
                case 'A':
                    if(args[currentArg].type != Arg::Type::FLOAT)
                    {
                        ThrowException<GInvalidArgumentException> ( GText( "Expected floating point argument as argument %d", 
                        currentArg+1).c_str(), file, func, line );
                        return false;
                    }
                    ++currentArg;
                    return true;
                case 'c':
                    if(args[currentArg].type != Arg::Type::CHAR)
                    {
                        ThrowException<GInvalidArgumentException> ( GText("Expected char argument as argument %d", 
                        currentArg+1 ).c_str(),  file, func, line );
                        return false;
                    }
                    ++currentArg;
                    return true;
                case 's':
                    if(args[currentArg].type != Arg::Type::STRING)
                    {
                        ThrowException<GInvalidArgumentException> ( GText("Expected string argument as argument %d", 
                        currentArg+1 ).c_str(),  file, func, line );
                        return false;
                    }
                    ++currentArg;
                    return true;
                case 'p':
                    if(args[currentArg].type != Arg::Type::POINTER)
                    {
                        ThrowException<GInvalidArgumentException> ( GText( "Expected pointer argument as argument %d", 
                        currentArg+1 ).c_str(),  file, func, line  );
                        return false;
                    }
                    ++currentArg;
                    return true;
                case '%': ++currentArg; break;
                default:
                      ThrowException<GInvalidArgumentException> ( GText( "Unspecified invalid argument(s) (n args = %d)", 
                        currentArg+1 ).c_str(),  file, func, line  );   
                return false;
                }
                return false;
            };
            findSpecifier(findSpecifier);
        }
        ++fmt;
    }
    if(currentArg < numArgs)
    {
         ThrowException<GInvalidArgumentException>  ( GText( "Too many arguments specified (currentArg = %d, numArgs = %d)", currentArg,  numArgs).c_str(),  
          file, func, line );
        return false;
    }
    return true;
}

std::pair<bool, std::string> // Success/failure, reason
doCheckFormat(const char *filename, int lineno, const char *function, const char *fmt, const Arg *args, const size_t numArgs)
{
    std::string reason;
    bool formatOk = false;
    try
    {
        formatOk = actuallyCheckFormat(filename, lineno, function, fmt, args, numArgs);
    }
    #ifdef HAS_LOGGING
    catch(GInvalidArgumentException &e)
    {
        std::stringstream str;
        str << "Error in format: " << fmt << ": " << e.what();
        reason = str.str();
        throw(e);
        formatOk = false;
    }
    catch(GMissingArgumentException &e)
    {
        std::stringstream str;
        str << "Error in format: " << fmt << ": " << e.what();
        reason = str.str();
        throw(e);
        formatOk = false;
    }
  catch(GException &e)
    {
        std::stringstream str;
        str << "Error in format: " << fmt << ": " << e.what();
        reason = str.str();
        throw(e);
        formatOk = false;
    }


    #endif
    catch(std::exception &e)
    {
        std::stringstream str;
        str << "Error when checking format: " << fmt << ": " << e.what();
        reason = str.str();
        throw(e);
        formatOk = false;
    }
    catch(...)
    {
        std::stringstream str;
        str << "Unknown error when checking format: " << fmt;
        reason = str.str();
        throw( std::invalid_argument(str.str().c_str()) );
        formatOk = false;
    }
    return {formatOk, reason};
}
}
