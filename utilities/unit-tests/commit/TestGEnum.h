#pragma once

//#include <testlib/TestBase.h>
#include <utilities/GEnum.h>

#undef HAS_LOGGING

#ifdef _WIN32
#include <gtest/gtest.h>
#else
#include <gtest-embc/gtest.h>
#endif

class TestGEnum : public testing::Test
{
public:
	G_ENUM(eTESTINT, { EN,TO,UNKNOWN = -1 });
};

class TestGEnum2
{
public:
	G_ENUM(eTESTINT2, { EN, TO=2 });
	eTESTINT2 fEnumInt2;
};