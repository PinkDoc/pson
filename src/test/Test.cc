#include "Value.hpp"
#include <gtest/gtest.h>

TEST(VALTEST, NullTest)
{
    pson::Value val(pson::JSON_NULL);
    EXPECT_EQ(val.AsNull(), 0);
}

TEST(VALTEST, NumberTest)
{
    pson::Value val(pson::JSON_NUMBER);
    val.AsNumber() = 10;
    EXPECT_EQ(val.AsNumber(), 10);
}

TEST(VALTEST, BoolTest)
{
    pson::Value val(pson::JSON_BOOL);
    val.AsBool() = true;
    EXPECT_EQ(val.AsBool(), true);
}

TEST(VALTEST, StringTest)
{
    pson::Value val(pson::JSON_STRING);
    val.AsString() = "Hello world";
    EXPECT_EQ(val.AsString(), "Hello world");
}

TEST(VALTEST, ArrayTest)
{
    pson::Value val(pson::JSON_ARRAY);
    val.AsArray().Push(NULL);
    EXPECT_EQ(val.AsArray().Size(), 1);
}

int main() {
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
}