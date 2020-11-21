#ifndef GFORMATTING_H
#define GFORMATTING_H

#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace GFormatting
{
class Arg
{
    /*
  private:
    union
    {
        uint64_t fUIntValue;
        int64_t fIntValue;
        double fDblValue;
        struct
        {
            const char *fStr;
            size_t fSize;
        } fStrValue;
        void *fPtrValue;
        char fCharValue;
    } u;
*/
  public:
    enum class Type
    {
        INT,
        FLOAT,
        CHAR,
        STRING,
        POINTER
    };
    Type type;
    template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true> Arg(Integer) : type(Type::INT)
    {
    }
    template<typename Value, typename std::enable_if_t<std::is_enum<Value>::value, bool> = true>
    Arg(Value) : type(Type::INT)
    {
    }
    template<typename Floating, std::enable_if_t<std::is_floating_point<Floating>::value, bool> = true>
    Arg(Floating) : type(Type::FLOAT)
    {
    }
    template<typename Pointer, std::enable_if_t<std::is_pointer<Pointer>::value, bool> = true>
    Arg(Pointer) : type(Type::POINTER)
    {
    }
    Arg(const char *) : type(Type::STRING) {}
    Arg(char *) : type(Type::STRING) {}
    Arg(char) : type(Type::CHAR) {}
    // other types
};
extern bool doCheckFormat( const char *file, const int line, const char *func,  const char *fmt, const Arg *args, const size_t numArgs);
template<typename... Args> constexpr bool checkFormat(   
    const char *file, const int line, const char *func, const char *fmt, const Args &... args)
{
    const Arg argArray[] = {args...};
    return doCheckFormat( file, line, func, fmt, argArray, sizeof...(Args));
}
constexpr bool checkFormat(  const char *, const int, const char *, const char *) 
{ 
    return true; 
    }

}
#endif // GFORMATTING_H
