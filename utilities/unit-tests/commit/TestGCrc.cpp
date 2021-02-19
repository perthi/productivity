// -*- mode: c++ -*-

/***************************************************
* @author Per Thomas Hille <pth@embc.no>           *
***************************************************/

#include "TestGCrc.h"
#include <utilities/GCrc.h>
#include <utilities/GRandom.h>
#include <memory>

#include <vector>

using std::vector;

TestGCrc::TestGCrc()
{

}


TestGCrc::~TestGCrc()
{

}


TEST_F( TestGCrc, crc_calc )
{
    vector<uint16_t> chk = { 12169, 21843, 45699, 48681, 22604, 18036, 30350, 62481 };


    for(int i=0; i < 8; i++ )
    {
        std::shared_ptr<GCrc> c =  std::make_shared<GCrc>(i);
        string r1 =  g_random()->RandomString(150);
        string r2 =  g_random()->RandomString(150);
        EXPECT_NE(r1, r2);
        auto crc1 = c->Crc(r1);
        auto crc2 = c->Crc(r2);

        EXPECT_NE(crc1, crc2);

        string s1 = "Lorem ipsum dolores sit amet";
        auto crc3 = c->Crc(s1);
        EXPECT_EQ(crc3, chk[i]);

        c->AddCrc16(r1);
        c->AddCrc16(r2);

        EXPECT_EQ( 152, r1.size() );
        EXPECT_EQ( 152, r2.size() );

        EXPECT_EQ(0, c->Crc(r1));
        EXPECT_EQ(0, c->Crc(r2));
        
        EXPECT_EQ(0, c->Crc(r1));
        EXPECT_EQ(0, c->Crc(r2));

        EXPECT_TRUE( c->CheckCrc(r1) );
        EXPECT_TRUE( c->CheckCrc(r2) );
    }
    
}


