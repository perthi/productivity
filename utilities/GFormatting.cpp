#include "GFormatting.h"
#include "exception/GException.h"
#include <iostream>

#include <cstdint>
namespace GFormatting
{
bool doCheckFormat(  const char *file, const int line, const char *func,  const char *fmt, const Arg *args, const size_t numArgs)
{
    // here we can access arguments by index
    size_t currentArg = 0;

    while(*fmt != '\0')
    {
        if(*fmt == '%')
        {
//            auto findSpecifier = [&currentArg, &fmt, args, numArgs](auto &&findSpec) constexpr
            auto findSpecifier = [&currentArg, &fmt, args, numArgs,  file, line, func ](auto&& findSpec)

            {
                if(currentArg >= numArgs)
                {
                    MISSING_ARGUMENT_EXCEPTION("Too few arguments, got %d, expected at least %d (from %s:%s:line[%d] )",
                     numArgs, numArgs+1, file, func, line  );
                    return false;
                }
                ++fmt;
                switch(*fmt)
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

}
