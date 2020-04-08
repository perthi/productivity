#pragma once

#include <testlib/TestBase.h>
#include <utilities/GEnum.h>

class TestGEnum : public TestBase
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