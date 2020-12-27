#ifndef GFORMATTING_H
#define GFORMATTING_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>

namespace GFormatting
{
#ifndef _WIN32
    namespace
    {
        template<class> inline constexpr bool always_false_v = false;
    }
#endif
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
            POINTER,
            INVALID
        };
        Type type;

        Arg(const char *) : type(Type::STRING) {}
        Arg(char *) : type(Type::STRING) {}
        Arg(char) : type(Type::CHAR) {}


        template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
        Arg(Integer) : type(Type::INT)
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
#ifndef _WIN32 // Not possible without c++17
        /* Compile error when type is not accepted */
        template<typename T,
                 std::enable_if_t<!std::is_integral<T>::value, bool> = true,
                 std::enable_if_t<!std::is_enum<T>::value, bool> = true,
                 std::enable_if_t<!std::is_floating_point<T>::value, bool> = true,
                 std::enable_if_t<!std::is_pointer<T>::value, bool> = true>
        Arg(T) : type(Type::INVALID) 
        { 
            static_assert(always_false_v<T>, "Invalid argument type for printf style formatting."); 
        }
#endif
    };
    extern std::pair<bool, std::string> doCheckFormat(const char *filename, int lineno, const char *function,
                                                      const char *fmt, const Arg *args, const size_t numArgs);
    template<typename... Args>
    static std::pair<bool, std::string> // success/failure, reason
    checkFormat(const char *filename, int lineno, const char *function, const char *fmt, const Args &...args)
    {
        const Arg argArray[] = {args...};
        return doCheckFormat(filename, lineno, function, fmt, argArray, sizeof...(Args));
    }

#if defined(__GNUC__) || defined(__clang__)
    static std::pair<bool, std::string> checkFormat(const char *, int, const char *, const char *)
        __attribute__((unused));
#endif
    static std::pair<bool, std::string> checkFormat(const char *, int, const char *, const char *)
    {
        return {true, std::string()};
    }
} // namespace GFormatting
#endif // GFORMATTING_H
