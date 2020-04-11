#include "TestGEnum.h"

G_ENUM(eTEST, { EN, TO , TRE = 3, FIRE,FEM = 055 , SEKS=0x66 })

G_ENUM(eTest2,
	{
		EN=101,
		TO = 102
	})
	

TEST_F(TestGEnum, get_char_ptr)
{
	EXPECT_TRUE(::strcmp(*eTEST::EN, "EN") == 0);
	EXPECT_TRUE(::strcmp(*eTEST::TO, "TO") == 0);
	EXPECT_TRUE(::strcmp(*eTEST::TRE, "TRE") == 0);
	EXPECT_TRUE(::strcmp(*eTEST::FIRE, "FIRE") == 0);
	EXPECT_TRUE(::strcmp(*eTEST::FEM, "FEM") == 0);
	EXPECT_TRUE(::strcmp(*eTEST::SEKS, "SEKS") == 0);
	EXPECT_TRUE(::strcmp(*eTest2::EN, "EN") == 0);
	EXPECT_TRUE(::strcmp(*eTest2::TO, "TO") == 0);
}

/*
TEST_F(TestGEnum, get_string)
{
	EXPECT_TRUE((*eTEST::EN) == "EN");
	EXPECT_TRUE((*eTEST::TO) == "TO");
	EXPECT_TRUE((*eTEST::TRE) == "TRE");
	EXPECT_TRUE((*eTEST::FIRE) == "FIRE");
	EXPECT_TRUE((*eTEST::FEM) == "FEM");
	EXPECT_TRUE((*eTEST::SEKS) == "SEKS");
}
*/

TEST_F(TestGEnum, Enum2Str)
{
	EXPECT_TRUE(::Enum2Str(eTEST::EN) == "EN");
	EXPECT_TRUE(::Enum2Str(eTEST::TO) == "TO");
	EXPECT_TRUE(::Enum2Str(eTEST::TRE) == "TRE");
	EXPECT_TRUE(::Enum2Str(eTEST::FIRE) == "FIRE");
	EXPECT_TRUE(::Enum2Str(eTEST::FEM) == "FEM");
	EXPECT_TRUE(::Enum2Str(eTEST::SEKS) == "SEKS");
}

TEST_F(TestGEnum, insideClass)
{
	// eTESTINT is a part of this class.
	EXPECT_TRUE(*eTEST::TO == Enum2Str(eTESTINT::TO));

	// eTESTINT2 in another class.
	EXPECT_EQ(::Enum2Str(eTEST::TO), Enum2Str(eTESTINT::TO));
	EXPECT_EQ(std::string(*eTest2::TO), TestGEnum2::Enum2ChPtr(TestGEnum2::eTESTINT2::TO));
}

TEST_F(TestGEnum, other)
{
//	EXPECT_TRUE(&eTEST::EN == *eTest2::EN);
	EXPECT_TRUE((int)(eTEST::EN) != (int)(eTest2::EN));
}
