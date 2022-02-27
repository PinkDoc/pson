#include <gtest/gtest.h>
#include "Value.hpp"
#include "Parser.hpp"
#include <stdio.h>
#include <assert.h>

/*
#define EXPECT_EQ(a, b) \
do {if (a != b)\
        {fprintf(stderr, "EQ assert at %d\n", __LINE__);\
            assert(false);} }while(0)\
*/


TEST(ParseTest, ParseBool_Right)
{
    std::string t = " true";
    std::string f = "   false";

    pson::Value saveVal1, saveVal2;

    pson::Parser parser1(t);
    bool ret1 = parser1.Parse(saveVal1);
    EXPECT_EQ(ret1, true);
    EXPECT_EQ(saveVal1.AsBool(), true);


    pson::Parser parser2(f);
    bool ret2 = parser2.Parse(saveVal2);
    EXPECT_EQ(ret2, true);
    EXPECT_EQ(saveVal2.AsBool(), false);
}

TEST(ParseTest, ParseNumber_Right)
{
    std::string number1 = "3.141512342352341";
    std::string number2 = "1.23125435353245";
    std::string number3 = "0.123123";
    std::string number4 = "-0.13453456e10";
    pson::Value v1, v2, v3, v4;

    pson::Parser parser(number1);
    auto r1 = parser.Parse(v1);
    EXPECT_EQ(r1, true);
    EXPECT_EQ(v1.AsNumber(), 3.141512342352341);

    parser.SetData(number2);
    auto r2 = parser.Parse(v2);
    EXPECT_EQ(r2, true);
    EXPECT_EQ(v2.AsNumber(), 1.23125435353245);

    parser.SetData(number3);
    auto r3 = parser.Parse(v3);
    EXPECT_EQ(r3, true);
    EXPECT_EQ(v3.AsNumber(), 0.123123);

    parser.SetData(number4);
    auto r4 = parser.Parse(v4);
    EXPECT_EQ(r4, true);
    EXPECT_EQ(v4.AsNumber(), -0.13453456e10);

}





int main()
{

    testing::InitGoogleTest();
    RUN_ALL_TESTS();

   // TEST();
}