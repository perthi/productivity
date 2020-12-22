#include "GFormatting.h"
#include "exception/GException.h"
#include <iostream>
#include <sstream>

#include <cstdint>
namespace GFormatting
{

bool actuallyCheckFormat(const char *file, int line, const char *func, const char *fmt, const Arg *args, const size_t numArgs)
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
                    MISSING_ARGUMENT_EXCEPTION("Too few arguments, got %d, expected at least %d (from %s:%s:line[%d] )",
                     numArgs, numArgs+1, file, func, line  );
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
                        INVALID_ARGUMENT_EXCEPTION("Expected integer argument as argument %d ( from %s:%s:line[%d] )",
                        currentArg+1, file, func, line  );
                        return false;
                    }
                    longSpecified = false;
                    ++currentArg;
                    return true;
                    default:
                        INVALID_ARGUMENT_EXCEPTION("Invalid format specifier at position %d ( from %s:%s:line[%d] )",
                                                   currentArg+1, file, func, line  );
                        return false;
                    }

                }
                switch(currChar)
                {
                case '\0':
                    // Too few arguments
                    MISSING_ARGUMENT_EXCEPTION("Too few arguments, got %d, expected at least %d ( from %s:%s:line[%d] )", 
                    numArgs, numArgs+1, file, func, line );
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
                        INVALID_ARGUMENT_EXCEPTION("Expected integer argument as argument %d ( from %s:%s:line[%d] )", 
                        currentArg+1, file, func, line  );
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
                        INVALID_ARGUMENT_EXCEPTION("Expected floating point argument as argument %d ( from %s:%s:line[%d] )", 
                        currentArg+1,   file, func, line  );
                        return false;
                    }
                    ++currentArg;
                    return true;
                case 'c':
                    if(args[currentArg].type != Arg::Type::CHAR)
                    {
                        INVALID_ARGUMENT_EXCEPTION("Expected char argument as argument %d ( from %s:%s:line[%d] )", 
                        currentArg+1,  file, func, line );
                        return false;
                    }
                    ++currentArg;
                    return true;
                case 's':
                    if(args[currentArg].type != Arg::Type::STRING)
                    {
                        INVALID_ARGUMENT_EXCEPTION("Expected string argument as argument %d ( from %s:%s:line[%d] ) ", 
                        currentArg+1, file, func, line );
                        return false;
                    }
                    ++currentArg;
                    return true;
                case 'p':
                    if(args[currentArg].type != Arg::Type::POINTER)
                    {
                        INVALID_ARGUMENT_EXCEPTION("Expected pointer argument as argument %d ( from %s:%s:line[%d] ) ", 
                        currentArg+1,   file, func, line  );
                        return false;
                    }
                    ++currentArg;
                    return true;
                case '%': ++currentArg; break;
                default:
                     //EXCEPTION("Unexpected format specifier ( from %s:%s:line[%d] ) ",   file, func, line  ); 
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
        INVALID_ARGUMENT_EXCEPTION("Too many arguments specified ( from %s:%s:line[%d] )", file, func, line );
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
    catch(GException &e)
    {
        std::stringstream str;
        str << "Error in format: " << fmt << ": " << e.what();
        reason = str.str();
        formatOk = false;
    }
    catch(std::exception &e)
    {
        std::stringstream str;
        str << "Error when checking format: " << fmt << ": " << e.what();
        reason = str.str();
        formatOk = false;
    }
    catch(...)
    {
        std::stringstream str;
        str << "Unknown error when checking format: " << fmt;
        reason = str.str();
        formatOk = false;
    }
    return {formatOk, reason};
}
}
