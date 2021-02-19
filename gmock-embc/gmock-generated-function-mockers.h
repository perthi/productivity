// This file was GENERATED by command:
//     pump.py gmock-generated-function-mockers.h.pump
// DO NOT EDIT BY HAND!!!

// Copyright 2007, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


// Google Mock - a framework for writing C++ mock classes.
//
// This file implements function mockers of various arities.

// GOOGLETEST_CM0002 DO NOT DELETE

#ifndef GMOCK_INCLUDE_GMOCK_GMOCK_GENERATED_FUNCTION_MOCKERS_H_
#define GMOCK_INCLUDE_GMOCK_GMOCK_GENERATED_FUNCTION_MOCKERS_H_

#include <functional>
#include <utility>

#include <gmock-embc/gmock-spec-builders.h>
#include <gmock-embc/internal/gmock-internal-utils.h>

namespace testing {
namespace internal {
// Removes the given pointer; this is a helper for the expectation setter method
// for parameterless matchers.
//
// We want to make sure that the user cannot set a parameterless expectation on
// overloaded methods, including methods which are overloaded on const. Example:
//
//   class MockClass {
//     MOCK_METHOD0(GetName, string&());
//     MOCK_CONST_METHOD0(GetName, const string&());
//   };
//
//   TEST() {
//     // This should be an error, as it's not clear which overload is expected.
//     EXPECT_CALL(mock, GetName).WillOnce(ReturnRef(value));
//   }
//
// Here are the generated expectation-setter methods:
//
//   class MockClass {
//     // Overload 1
//     MockSpec<string&()> gmock_GetName() { ... }
//     // Overload 2. Declared const so that the compiler will generate an
//     // error when trying to resolve between this and overload 4 in
//     // 'gmock_GetName(WithoutMatchers(), nullptr)'.
//     MockSpec<string&()> gmock_GetName(
//         const WithoutMatchers&, const Function<string&()>*) const {
//       // Removes const from this, calls overload 1
//       return AdjustConstness_(this)->gmock_GetName();
//     }
//
//     // Overload 3
//     const string& gmock_GetName() const { ... }
//     // Overload 4
//     MockSpec<const string&()> gmock_GetName(
//         const WithoutMatchers&, const Function<const string&()>*) const {
//       // Does not remove const, calls overload 3
//       return AdjustConstness_const(this)->gmock_GetName();
//     }
//   }
//
template <typename MockType>
const MockType* AdjustConstness_const(const MockType* mock) {
  return mock;
}

// Removes const from and returns the given pointer; this is a helper for the
// expectation setter method for parameterless matchers.
template <typename MockType>
MockType* AdjustConstness_(const MockType* mock) {
  return const_cast<MockType*>(mock);
}

}  // namespace internal

// The style guide prohibits "using" statements in a namespace scope
// inside a header file.  However, the FunctionMocker class template
// is meant to be defined in the ::testing namespace.  The following
// line is just a trick for working around a bug in MSVC 8.0, which
// cannot handle it if we define FunctionMocker in ::testing.
using internal::FunctionMocker;

// GMOCK_RESULT_(tn, F) expands to the result type of function type F.
// We define this as a variadic macro in case F contains unprotected
// commas (the same reason that we use variadic macros in other places
// in this file).
// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_RESULT_(tn, ...) \
    tn ::testing::internal::Function<__VA_ARGS__>::Result

// The type of argument N of the given function type.
// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_ARG_(tn, N, ...) \
    tn ::testing::internal::Function<__VA_ARGS__>::template Arg<N-1>::type

// The matcher type for argument N of the given function type.
// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_MATCHER_(tn, N, ...) \
    const ::testing::Matcher<GMOCK_ARG_(tn, N, __VA_ARGS__)>&

// The variable for mocking the given method.
// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_MOCKER_(arity, constness, Method) \
    GTEST_CONCAT_TOKEN_(gmock##constness##arity##_##Method##_, __LINE__)

// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_METHOD0_(tn, constness, ct, Method, ...) \
  static_assert(0 == \
      ::testing::internal::Function<__VA_ARGS__>::ArgumentCount, \
      "MOCK_METHOD<N> must match argument count.");\
  GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      ) constness { \
    GMOCK_MOCKER_(0, constness, Method).SetOwnerAndName(this, #Method); \
    return GMOCK_MOCKER_(0, constness, Method).Invoke(); \
  } \
  ::testing::MockSpec<__VA_ARGS__> \
      gmock_##Method() constness { \
    GMOCK_MOCKER_(0, constness, Method).RegisterOwner(this); \
    return GMOCK_MOCKER_(0, constness, Method).With(); \
  } \
  ::testing::MockSpec<__VA_ARGS__> gmock_##Method( \
      const ::testing::internal::WithoutMatchers&, \
      constness ::testing::internal::Function<__VA_ARGS__>* ) const { \
        return ::testing::internal::AdjustConstness_##constness(this)-> \
            gmock_##Method(); \
      } \
  mutable ::testing::FunctionMocker<__VA_ARGS__> GMOCK_MOCKER_(0, constness, \
      Method)

// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_METHOD1_(tn, constness, ct, Method, ...) \
  static_assert(1 == \
      ::testing::internal::Function<__VA_ARGS__>::ArgumentCount, \
      "MOCK_METHOD<N> must match argument count.");\
  GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1) constness { \
    GMOCK_MOCKER_(1, constness, Method).SetOwnerAndName(this, #Method); \
    return GMOCK_MOCKER_(1, constness, \
        Method).Invoke(::std::forward<GMOCK_ARG_(tn, 1, \
        __VA_ARGS__)>(gmock_a1)); \
  } \
  ::testing::MockSpec<__VA_ARGS__> \
      gmock_##Method(GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1) constness { \
    GMOCK_MOCKER_(1, constness, Method).RegisterOwner(this); \
    return GMOCK_MOCKER_(1, constness, Method).With(gmock_a1); \
  } \
  ::testing::MockSpec<__VA_ARGS__> gmock_##Method( \
      const ::testing::internal::WithoutMatchers&, \
      constness ::testing::internal::Function<__VA_ARGS__>* ) const { \
        return ::testing::internal::AdjustConstness_##constness(this)-> \
            gmock_##Method(::testing::A<GMOCK_ARG_(tn, 1, __VA_ARGS__)>()); \
      } \
  mutable ::testing::FunctionMocker<__VA_ARGS__> GMOCK_MOCKER_(1, constness, \
      Method)

// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_METHOD2_(tn, constness, ct, Method, ...) \
  static_assert(2 == \
      ::testing::internal::Function<__VA_ARGS__>::ArgumentCount, \
      "MOCK_METHOD<N> must match argument count.");\
  GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, GMOCK_ARG_(tn, 2, \
          __VA_ARGS__) gmock_a2) constness { \
    GMOCK_MOCKER_(2, constness, Method).SetOwnerAndName(this, #Method); \
    return GMOCK_MOCKER_(2, constness, \
        Method).Invoke(::std::forward<GMOCK_ARG_(tn, 1, \
        __VA_ARGS__)>(gmock_a1), \
  ::std::forward<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(gmock_a2)); \
  } \
  ::testing::MockSpec<__VA_ARGS__> \
      gmock_##Method(GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
                     GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2) constness { \
    GMOCK_MOCKER_(2, constness, Method).RegisterOwner(this); \
    return GMOCK_MOCKER_(2, constness, Method).With(gmock_a1, gmock_a2); \
  } \
  ::testing::MockSpec<__VA_ARGS__> gmock_##Method( \
      const ::testing::internal::WithoutMatchers&, \
      constness ::testing::internal::Function<__VA_ARGS__>* ) const { \
        return ::testing::internal::AdjustConstness_##constness(this)-> \
            gmock_##Method(::testing::A<GMOCK_ARG_(tn, 1, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 2, __VA_ARGS__)>()); \
      } \
  mutable ::testing::FunctionMocker<__VA_ARGS__> GMOCK_MOCKER_(2, constness, \
      Method)

// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_METHOD3_(tn, constness, ct, Method, ...) \
  static_assert(3 == \
      ::testing::internal::Function<__VA_ARGS__>::ArgumentCount, \
      "MOCK_METHOD<N> must match argument count.");\
  GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, GMOCK_ARG_(tn, 2, \
          __VA_ARGS__) gmock_a2, GMOCK_ARG_(tn, 3, \
          __VA_ARGS__) gmock_a3) constness { \
    GMOCK_MOCKER_(3, constness, Method).SetOwnerAndName(this, #Method); \
    return GMOCK_MOCKER_(3, constness, \
        Method).Invoke(::std::forward<GMOCK_ARG_(tn, 1, \
        __VA_ARGS__)>(gmock_a1), \
  ::std::forward<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(gmock_a2), \
  ::std::forward<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(gmock_a3)); \
  } \
  ::testing::MockSpec<__VA_ARGS__> \
      gmock_##Method(GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
                     GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
                     GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3) constness { \
    GMOCK_MOCKER_(3, constness, Method).RegisterOwner(this); \
    return GMOCK_MOCKER_(3, constness, Method).With(gmock_a1, gmock_a2, \
        gmock_a3); \
  } \
  ::testing::MockSpec<__VA_ARGS__> gmock_##Method( \
      const ::testing::internal::WithoutMatchers&, \
      constness ::testing::internal::Function<__VA_ARGS__>* ) const { \
        return ::testing::internal::AdjustConstness_##constness(this)-> \
            gmock_##Method(::testing::A<GMOCK_ARG_(tn, 1, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 3, __VA_ARGS__)>()); \
      } \
  mutable ::testing::FunctionMocker<__VA_ARGS__> GMOCK_MOCKER_(3, constness, \
      Method)

// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_METHOD4_(tn, constness, ct, Method, ...) \
  static_assert(4 == \
      ::testing::internal::Function<__VA_ARGS__>::ArgumentCount, \
      "MOCK_METHOD<N> must match argument count.");\
  GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, GMOCK_ARG_(tn, 2, \
          __VA_ARGS__) gmock_a2, GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
          GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4) constness { \
    GMOCK_MOCKER_(4, constness, Method).SetOwnerAndName(this, #Method); \
    return GMOCK_MOCKER_(4, constness, \
        Method).Invoke(::std::forward<GMOCK_ARG_(tn, 1, \
        __VA_ARGS__)>(gmock_a1), \
  ::std::forward<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(gmock_a2), \
  ::std::forward<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(gmock_a3), \
  ::std::forward<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(gmock_a4)); \
  } \
  ::testing::MockSpec<__VA_ARGS__> \
      gmock_##Method(GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
                     GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
                     GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3, \
                     GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4) constness { \
    GMOCK_MOCKER_(4, constness, Method).RegisterOwner(this); \
    return GMOCK_MOCKER_(4, constness, Method).With(gmock_a1, gmock_a2, \
        gmock_a3, gmock_a4); \
  } \
  ::testing::MockSpec<__VA_ARGS__> gmock_##Method( \
      const ::testing::internal::WithoutMatchers&, \
      constness ::testing::internal::Function<__VA_ARGS__>* ) const { \
        return ::testing::internal::AdjustConstness_##constness(this)-> \
            gmock_##Method(::testing::A<GMOCK_ARG_(tn, 1, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 4, __VA_ARGS__)>()); \
      } \
  mutable ::testing::FunctionMocker<__VA_ARGS__> GMOCK_MOCKER_(4, constness, \
      Method)

// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_METHOD5_(tn, constness, ct, Method, ...) \
  static_assert(5 == \
      ::testing::internal::Function<__VA_ARGS__>::ArgumentCount, \
      "MOCK_METHOD<N> must match argument count.");\
  GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, GMOCK_ARG_(tn, 2, \
          __VA_ARGS__) gmock_a2, GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
          GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, GMOCK_ARG_(tn, 5, \
          __VA_ARGS__) gmock_a5) constness { \
    GMOCK_MOCKER_(5, constness, Method).SetOwnerAndName(this, #Method); \
    return GMOCK_MOCKER_(5, constness, \
        Method).Invoke(::std::forward<GMOCK_ARG_(tn, 1, \
        __VA_ARGS__)>(gmock_a1), \
  ::std::forward<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(gmock_a2), \
  ::std::forward<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(gmock_a3), \
  ::std::forward<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(gmock_a4), \
  ::std::forward<GMOCK_ARG_(tn, 5, __VA_ARGS__)>(gmock_a5)); \
  } \
  ::testing::MockSpec<__VA_ARGS__> \
      gmock_##Method(GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
                     GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
                     GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3, \
                     GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4, \
                     GMOCK_MATCHER_(tn, 5, __VA_ARGS__) gmock_a5) constness { \
    GMOCK_MOCKER_(5, constness, Method).RegisterOwner(this); \
    return GMOCK_MOCKER_(5, constness, Method).With(gmock_a1, gmock_a2, \
        gmock_a3, gmock_a4, gmock_a5); \
  } \
  ::testing::MockSpec<__VA_ARGS__> gmock_##Method( \
      const ::testing::internal::WithoutMatchers&, \
      constness ::testing::internal::Function<__VA_ARGS__>* ) const { \
        return ::testing::internal::AdjustConstness_##constness(this)-> \
            gmock_##Method(::testing::A<GMOCK_ARG_(tn, 1, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 5, __VA_ARGS__)>()); \
      } \
  mutable ::testing::FunctionMocker<__VA_ARGS__> GMOCK_MOCKER_(5, constness, \
      Method)

// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_METHOD6_(tn, constness, ct, Method, ...) \
  static_assert(6 == \
      ::testing::internal::Function<__VA_ARGS__>::ArgumentCount, \
      "MOCK_METHOD<N> must match argument count.");\
  GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, GMOCK_ARG_(tn, 2, \
          __VA_ARGS__) gmock_a2, GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
          GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, GMOCK_ARG_(tn, 5, \
          __VA_ARGS__) gmock_a5, GMOCK_ARG_(tn, 6, \
          __VA_ARGS__) gmock_a6) constness { \
    GMOCK_MOCKER_(6, constness, Method).SetOwnerAndName(this, #Method); \
    return GMOCK_MOCKER_(6, constness, \
        Method).Invoke(::std::forward<GMOCK_ARG_(tn, 1, \
        __VA_ARGS__)>(gmock_a1), \
  ::std::forward<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(gmock_a2), \
  ::std::forward<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(gmock_a3), \
  ::std::forward<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(gmock_a4), \
  ::std::forward<GMOCK_ARG_(tn, 5, __VA_ARGS__)>(gmock_a5), \
  ::std::forward<GMOCK_ARG_(tn, 6, __VA_ARGS__)>(gmock_a6)); \
  } \
  ::testing::MockSpec<__VA_ARGS__> \
      gmock_##Method(GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
                     GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
                     GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3, \
                     GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4, \
                     GMOCK_MATCHER_(tn, 5, __VA_ARGS__) gmock_a5, \
                     GMOCK_MATCHER_(tn, 6, __VA_ARGS__) gmock_a6) constness { \
    GMOCK_MOCKER_(6, constness, Method).RegisterOwner(this); \
    return GMOCK_MOCKER_(6, constness, Method).With(gmock_a1, gmock_a2, \
        gmock_a3, gmock_a4, gmock_a5, gmock_a6); \
  } \
  ::testing::MockSpec<__VA_ARGS__> gmock_##Method( \
      const ::testing::internal::WithoutMatchers&, \
      constness ::testing::internal::Function<__VA_ARGS__>* ) const { \
        return ::testing::internal::AdjustConstness_##constness(this)-> \
            gmock_##Method(::testing::A<GMOCK_ARG_(tn, 1, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 5, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 6, __VA_ARGS__)>()); \
      } \
  mutable ::testing::FunctionMocker<__VA_ARGS__> GMOCK_MOCKER_(6, constness, \
      Method)

// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_METHOD7_(tn, constness, ct, Method, ...) \
  static_assert(7 == \
      ::testing::internal::Function<__VA_ARGS__>::ArgumentCount, \
      "MOCK_METHOD<N> must match argument count.");\
  GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, GMOCK_ARG_(tn, 2, \
          __VA_ARGS__) gmock_a2, GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
          GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, GMOCK_ARG_(tn, 5, \
          __VA_ARGS__) gmock_a5, GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6, \
          GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7) constness { \
    GMOCK_MOCKER_(7, constness, Method).SetOwnerAndName(this, #Method); \
    return GMOCK_MOCKER_(7, constness, \
        Method).Invoke(::std::forward<GMOCK_ARG_(tn, 1, \
        __VA_ARGS__)>(gmock_a1), \
  ::std::forward<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(gmock_a2), \
  ::std::forward<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(gmock_a3), \
  ::std::forward<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(gmock_a4), \
  ::std::forward<GMOCK_ARG_(tn, 5, __VA_ARGS__)>(gmock_a5), \
  ::std::forward<GMOCK_ARG_(tn, 6, __VA_ARGS__)>(gmock_a6), \
  ::std::forward<GMOCK_ARG_(tn, 7, __VA_ARGS__)>(gmock_a7)); \
  } \
  ::testing::MockSpec<__VA_ARGS__> \
      gmock_##Method(GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
                     GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
                     GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3, \
                     GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4, \
                     GMOCK_MATCHER_(tn, 5, __VA_ARGS__) gmock_a5, \
                     GMOCK_MATCHER_(tn, 6, __VA_ARGS__) gmock_a6, \
                     GMOCK_MATCHER_(tn, 7, __VA_ARGS__) gmock_a7) constness { \
    GMOCK_MOCKER_(7, constness, Method).RegisterOwner(this); \
    return GMOCK_MOCKER_(7, constness, Method).With(gmock_a1, gmock_a2, \
        gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7); \
  } \
  ::testing::MockSpec<__VA_ARGS__> gmock_##Method( \
      const ::testing::internal::WithoutMatchers&, \
      constness ::testing::internal::Function<__VA_ARGS__>* ) const { \
        return ::testing::internal::AdjustConstness_##constness(this)-> \
            gmock_##Method(::testing::A<GMOCK_ARG_(tn, 1, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 5, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 6, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 7, __VA_ARGS__)>()); \
      } \
  mutable ::testing::FunctionMocker<__VA_ARGS__> GMOCK_MOCKER_(7, constness, \
      Method)

// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_METHOD8_(tn, constness, ct, Method, ...) \
  static_assert(8 == \
      ::testing::internal::Function<__VA_ARGS__>::ArgumentCount, \
      "MOCK_METHOD<N> must match argument count.");\
  GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, GMOCK_ARG_(tn, 2, \
          __VA_ARGS__) gmock_a2, GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
          GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, GMOCK_ARG_(tn, 5, \
          __VA_ARGS__) gmock_a5, GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6, \
          GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7, GMOCK_ARG_(tn, 8, \
          __VA_ARGS__) gmock_a8) constness { \
    GMOCK_MOCKER_(8, constness, Method).SetOwnerAndName(this, #Method); \
    return GMOCK_MOCKER_(8, constness, \
        Method).Invoke(::std::forward<GMOCK_ARG_(tn, 1, \
        __VA_ARGS__)>(gmock_a1), \
  ::std::forward<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(gmock_a2), \
  ::std::forward<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(gmock_a3), \
  ::std::forward<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(gmock_a4), \
  ::std::forward<GMOCK_ARG_(tn, 5, __VA_ARGS__)>(gmock_a5), \
  ::std::forward<GMOCK_ARG_(tn, 6, __VA_ARGS__)>(gmock_a6), \
  ::std::forward<GMOCK_ARG_(tn, 7, __VA_ARGS__)>(gmock_a7), \
  ::std::forward<GMOCK_ARG_(tn, 8, __VA_ARGS__)>(gmock_a8)); \
  } \
  ::testing::MockSpec<__VA_ARGS__> \
      gmock_##Method(GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
                     GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
                     GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3, \
                     GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4, \
                     GMOCK_MATCHER_(tn, 5, __VA_ARGS__) gmock_a5, \
                     GMOCK_MATCHER_(tn, 6, __VA_ARGS__) gmock_a6, \
                     GMOCK_MATCHER_(tn, 7, __VA_ARGS__) gmock_a7, \
                     GMOCK_MATCHER_(tn, 8, __VA_ARGS__) gmock_a8) constness { \
    GMOCK_MOCKER_(8, constness, Method).RegisterOwner(this); \
    return GMOCK_MOCKER_(8, constness, Method).With(gmock_a1, gmock_a2, \
        gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8); \
  } \
  ::testing::MockSpec<__VA_ARGS__> gmock_##Method( \
      const ::testing::internal::WithoutMatchers&, \
      constness ::testing::internal::Function<__VA_ARGS__>* ) const { \
        return ::testing::internal::AdjustConstness_##constness(this)-> \
            gmock_##Method(::testing::A<GMOCK_ARG_(tn, 1, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 5, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 6, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 7, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 8, __VA_ARGS__)>()); \
      } \
  mutable ::testing::FunctionMocker<__VA_ARGS__> GMOCK_MOCKER_(8, constness, \
      Method)

// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_METHOD9_(tn, constness, ct, Method, ...) \
  static_assert(9 == \
      ::testing::internal::Function<__VA_ARGS__>::ArgumentCount, \
      "MOCK_METHOD<N> must match argument count.");\
  GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, GMOCK_ARG_(tn, 2, \
          __VA_ARGS__) gmock_a2, GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
          GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, GMOCK_ARG_(tn, 5, \
          __VA_ARGS__) gmock_a5, GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6, \
          GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7, GMOCK_ARG_(tn, 8, \
          __VA_ARGS__) gmock_a8, GMOCK_ARG_(tn, 9, \
          __VA_ARGS__) gmock_a9) constness { \
    GMOCK_MOCKER_(9, constness, Method).SetOwnerAndName(this, #Method); \
    return GMOCK_MOCKER_(9, constness, \
        Method).Invoke(::std::forward<GMOCK_ARG_(tn, 1, \
        __VA_ARGS__)>(gmock_a1), \
  ::std::forward<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(gmock_a2), \
  ::std::forward<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(gmock_a3), \
  ::std::forward<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(gmock_a4), \
  ::std::forward<GMOCK_ARG_(tn, 5, __VA_ARGS__)>(gmock_a5), \
  ::std::forward<GMOCK_ARG_(tn, 6, __VA_ARGS__)>(gmock_a6), \
  ::std::forward<GMOCK_ARG_(tn, 7, __VA_ARGS__)>(gmock_a7), \
  ::std::forward<GMOCK_ARG_(tn, 8, __VA_ARGS__)>(gmock_a8), \
  ::std::forward<GMOCK_ARG_(tn, 9, __VA_ARGS__)>(gmock_a9)); \
  } \
  ::testing::MockSpec<__VA_ARGS__> \
      gmock_##Method(GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
                     GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
                     GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3, \
                     GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4, \
                     GMOCK_MATCHER_(tn, 5, __VA_ARGS__) gmock_a5, \
                     GMOCK_MATCHER_(tn, 6, __VA_ARGS__) gmock_a6, \
                     GMOCK_MATCHER_(tn, 7, __VA_ARGS__) gmock_a7, \
                     GMOCK_MATCHER_(tn, 8, __VA_ARGS__) gmock_a8, \
                     GMOCK_MATCHER_(tn, 9, __VA_ARGS__) gmock_a9) constness { \
    GMOCK_MOCKER_(9, constness, Method).RegisterOwner(this); \
    return GMOCK_MOCKER_(9, constness, Method).With(gmock_a1, gmock_a2, \
        gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, \
        gmock_a9); \
  } \
  ::testing::MockSpec<__VA_ARGS__> gmock_##Method( \
      const ::testing::internal::WithoutMatchers&, \
      constness ::testing::internal::Function<__VA_ARGS__>* ) const { \
        return ::testing::internal::AdjustConstness_##constness(this)-> \
            gmock_##Method(::testing::A<GMOCK_ARG_(tn, 1, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 5, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 6, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 7, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 8, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 9, __VA_ARGS__)>()); \
      } \
  mutable ::testing::FunctionMocker<__VA_ARGS__> GMOCK_MOCKER_(9, constness, \
      Method)

// INTERNAL IMPLEMENTATION - DON'T USE IN USER CODE!!!
#define GMOCK_METHOD10_(tn, constness, ct, Method, ...) \
  static_assert(10 == \
      ::testing::internal::Function<__VA_ARGS__>::ArgumentCount, \
      "MOCK_METHOD<N> must match argument count.");\
  GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, GMOCK_ARG_(tn, 2, \
          __VA_ARGS__) gmock_a2, GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
          GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, GMOCK_ARG_(tn, 5, \
          __VA_ARGS__) gmock_a5, GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6, \
          GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7, GMOCK_ARG_(tn, 8, \
          __VA_ARGS__) gmock_a8, GMOCK_ARG_(tn, 9, __VA_ARGS__) gmock_a9, \
          GMOCK_ARG_(tn, 10, __VA_ARGS__) gmock_a10) constness { \
    GMOCK_MOCKER_(10, constness, Method).SetOwnerAndName(this, #Method); \
    return GMOCK_MOCKER_(10, constness, \
        Method).Invoke(::std::forward<GMOCK_ARG_(tn, 1, \
        __VA_ARGS__)>(gmock_a1), \
  ::std::forward<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(gmock_a2), \
  ::std::forward<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(gmock_a3), \
  ::std::forward<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(gmock_a4), \
  ::std::forward<GMOCK_ARG_(tn, 5, __VA_ARGS__)>(gmock_a5), \
  ::std::forward<GMOCK_ARG_(tn, 6, __VA_ARGS__)>(gmock_a6), \
  ::std::forward<GMOCK_ARG_(tn, 7, __VA_ARGS__)>(gmock_a7), \
  ::std::forward<GMOCK_ARG_(tn, 8, __VA_ARGS__)>(gmock_a8), \
  ::std::forward<GMOCK_ARG_(tn, 9, __VA_ARGS__)>(gmock_a9), \
  ::std::forward<GMOCK_ARG_(tn, 10, __VA_ARGS__)>(gmock_a10)); \
  } \
  ::testing::MockSpec<__VA_ARGS__> \
      gmock_##Method(GMOCK_MATCHER_(tn, 1, __VA_ARGS__) gmock_a1, \
                     GMOCK_MATCHER_(tn, 2, __VA_ARGS__) gmock_a2, \
                     GMOCK_MATCHER_(tn, 3, __VA_ARGS__) gmock_a3, \
                     GMOCK_MATCHER_(tn, 4, __VA_ARGS__) gmock_a4, \
                     GMOCK_MATCHER_(tn, 5, __VA_ARGS__) gmock_a5, \
                     GMOCK_MATCHER_(tn, 6, __VA_ARGS__) gmock_a6, \
                     GMOCK_MATCHER_(tn, 7, __VA_ARGS__) gmock_a7, \
                     GMOCK_MATCHER_(tn, 8, __VA_ARGS__) gmock_a8, \
                     GMOCK_MATCHER_(tn, 9, __VA_ARGS__) gmock_a9, \
                     GMOCK_MATCHER_(tn, 10, \
                         __VA_ARGS__) gmock_a10) constness { \
    GMOCK_MOCKER_(10, constness, Method).RegisterOwner(this); \
    return GMOCK_MOCKER_(10, constness, Method).With(gmock_a1, gmock_a2, \
        gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9, \
        gmock_a10); \
  } \
  ::testing::MockSpec<__VA_ARGS__> gmock_##Method( \
      const ::testing::internal::WithoutMatchers&, \
      constness ::testing::internal::Function<__VA_ARGS__>* ) const { \
        return ::testing::internal::AdjustConstness_##constness(this)-> \
            gmock_##Method(::testing::A<GMOCK_ARG_(tn, 1, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 2, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 3, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 4, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 5, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 6, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 7, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 8, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 9, __VA_ARGS__)>(), \
                     ::testing::A<GMOCK_ARG_(tn, 10, __VA_ARGS__)>()); \
      } \
  mutable ::testing::FunctionMocker<__VA_ARGS__> GMOCK_MOCKER_(10, constness, \
      Method)

#define MOCK_METHOD0(m, ...) GMOCK_METHOD0_(, , , m, __VA_ARGS__)
#define MOCK_METHOD1(m, ...) GMOCK_METHOD1_(, , , m, __VA_ARGS__)
#define MOCK_METHOD2(m, ...) GMOCK_METHOD2_(, , , m, __VA_ARGS__)
#define MOCK_METHOD3(m, ...) GMOCK_METHOD3_(, , , m, __VA_ARGS__)
#define MOCK_METHOD4(m, ...) GMOCK_METHOD4_(, , , m, __VA_ARGS__)
#define MOCK_METHOD5(m, ...) GMOCK_METHOD5_(, , , m, __VA_ARGS__)
#define MOCK_METHOD6(m, ...) GMOCK_METHOD6_(, , , m, __VA_ARGS__)
#define MOCK_METHOD7(m, ...) GMOCK_METHOD7_(, , , m, __VA_ARGS__)
#define MOCK_METHOD8(m, ...) GMOCK_METHOD8_(, , , m, __VA_ARGS__)
#define MOCK_METHOD9(m, ...) GMOCK_METHOD9_(, , , m, __VA_ARGS__)
#define MOCK_METHOD10(m, ...) GMOCK_METHOD10_(, , , m, __VA_ARGS__)

#define MOCK_CONST_METHOD0(m, ...) GMOCK_METHOD0_(, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD1(m, ...) GMOCK_METHOD1_(, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD2(m, ...) GMOCK_METHOD2_(, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD3(m, ...) GMOCK_METHOD3_(, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD4(m, ...) GMOCK_METHOD4_(, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD5(m, ...) GMOCK_METHOD5_(, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD6(m, ...) GMOCK_METHOD6_(, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD7(m, ...) GMOCK_METHOD7_(, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD8(m, ...) GMOCK_METHOD8_(, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD9(m, ...) GMOCK_METHOD9_(, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD10(m, ...) GMOCK_METHOD10_(, const, , m, __VA_ARGS__)

#define MOCK_METHOD0_T(m, ...) GMOCK_METHOD0_(typename, , , m, __VA_ARGS__)
#define MOCK_METHOD1_T(m, ...) GMOCK_METHOD1_(typename, , , m, __VA_ARGS__)
#define MOCK_METHOD2_T(m, ...) GMOCK_METHOD2_(typename, , , m, __VA_ARGS__)
#define MOCK_METHOD3_T(m, ...) GMOCK_METHOD3_(typename, , , m, __VA_ARGS__)
#define MOCK_METHOD4_T(m, ...) GMOCK_METHOD4_(typename, , , m, __VA_ARGS__)
#define MOCK_METHOD5_T(m, ...) GMOCK_METHOD5_(typename, , , m, __VA_ARGS__)
#define MOCK_METHOD6_T(m, ...) GMOCK_METHOD6_(typename, , , m, __VA_ARGS__)
#define MOCK_METHOD7_T(m, ...) GMOCK_METHOD7_(typename, , , m, __VA_ARGS__)
#define MOCK_METHOD8_T(m, ...) GMOCK_METHOD8_(typename, , , m, __VA_ARGS__)
#define MOCK_METHOD9_T(m, ...) GMOCK_METHOD9_(typename, , , m, __VA_ARGS__)
#define MOCK_METHOD10_T(m, ...) GMOCK_METHOD10_(typename, , , m, __VA_ARGS__)

#define MOCK_CONST_METHOD0_T(m, ...) \
    GMOCK_METHOD0_(typename, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD1_T(m, ...) \
    GMOCK_METHOD1_(typename, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD2_T(m, ...) \
    GMOCK_METHOD2_(typename, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD3_T(m, ...) \
    GMOCK_METHOD3_(typename, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD4_T(m, ...) \
    GMOCK_METHOD4_(typename, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD5_T(m, ...) \
    GMOCK_METHOD5_(typename, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD6_T(m, ...) \
    GMOCK_METHOD6_(typename, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD7_T(m, ...) \
    GMOCK_METHOD7_(typename, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD8_T(m, ...) \
    GMOCK_METHOD8_(typename, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD9_T(m, ...) \
    GMOCK_METHOD9_(typename, const, , m, __VA_ARGS__)
#define MOCK_CONST_METHOD10_T(m, ...) \
    GMOCK_METHOD10_(typename, const, , m, __VA_ARGS__)

#define MOCK_METHOD0_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD0_(, , ct, m, __VA_ARGS__)
#define MOCK_METHOD1_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD1_(, , ct, m, __VA_ARGS__)
#define MOCK_METHOD2_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD2_(, , ct, m, __VA_ARGS__)
#define MOCK_METHOD3_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD3_(, , ct, m, __VA_ARGS__)
#define MOCK_METHOD4_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD4_(, , ct, m, __VA_ARGS__)
#define MOCK_METHOD5_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD5_(, , ct, m, __VA_ARGS__)
#define MOCK_METHOD6_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD6_(, , ct, m, __VA_ARGS__)
#define MOCK_METHOD7_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD7_(, , ct, m, __VA_ARGS__)
#define MOCK_METHOD8_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD8_(, , ct, m, __VA_ARGS__)
#define MOCK_METHOD9_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD9_(, , ct, m, __VA_ARGS__)
#define MOCK_METHOD10_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD10_(, , ct, m, __VA_ARGS__)

#define MOCK_CONST_METHOD0_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD0_(, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD1_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD1_(, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD2_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD2_(, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD3_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD3_(, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD4_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD4_(, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD5_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD5_(, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD6_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD6_(, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD7_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD7_(, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD8_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD8_(, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD9_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD9_(, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD10_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD10_(, const, ct, m, __VA_ARGS__)

#define MOCK_METHOD0_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD0_(typename, , ct, m, __VA_ARGS__)
#define MOCK_METHOD1_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD1_(typename, , ct, m, __VA_ARGS__)
#define MOCK_METHOD2_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD2_(typename, , ct, m, __VA_ARGS__)
#define MOCK_METHOD3_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD3_(typename, , ct, m, __VA_ARGS__)
#define MOCK_METHOD4_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD4_(typename, , ct, m, __VA_ARGS__)
#define MOCK_METHOD5_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD5_(typename, , ct, m, __VA_ARGS__)
#define MOCK_METHOD6_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD6_(typename, , ct, m, __VA_ARGS__)
#define MOCK_METHOD7_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD7_(typename, , ct, m, __VA_ARGS__)
#define MOCK_METHOD8_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD8_(typename, , ct, m, __VA_ARGS__)
#define MOCK_METHOD9_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD9_(typename, , ct, m, __VA_ARGS__)
#define MOCK_METHOD10_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD10_(typename, , ct, m, __VA_ARGS__)

#define MOCK_CONST_METHOD0_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD0_(typename, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD1_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD1_(typename, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD2_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD2_(typename, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD3_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD3_(typename, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD4_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD4_(typename, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD5_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD5_(typename, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD6_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD6_(typename, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD7_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD7_(typename, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD8_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD8_(typename, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD9_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD9_(typename, const, ct, m, __VA_ARGS__)
#define MOCK_CONST_METHOD10_T_WITH_CALLTYPE(ct, m, ...) \
    GMOCK_METHOD10_(typename, const, ct, m, __VA_ARGS__)

}  // namespace testing

#endif  // GMOCK_INCLUDE_GMOCK_GMOCK_GENERATED_FUNCTION_MOCKERS_H_
