#include "../Generater.hpp"
#include <gtest/gtest.h>

using pson::Value;

TEST(GenTest, NullTest)
{
    Value v(pson::JSON_NULL);
    std::string ret = pson::PrintValue(v);
    EXPECT_EQ(ret, "null");
}

TEST(GenTest, BoolTest)
{
    Value v(pson::JSON_BOOL);
    v.AsBool() = false;
    std::string ret = pson::PrintValue(v);
    EXPECT_EQ(ret, "false");
    v.AsBool() = true;
    ret = pson::PrintValue(v);
    EXPECT_EQ(ret, "true");
}

TEST(GenTest, NumberTest)
{
    Value v(pson::JSON_NUMBER);
    v.AsNumber() = 1122334455;
    EXPECT_EQ( pson::PrintValue(v),"1122334455.000000");
}

TEST(GenTest, ArrayTest)
{
    Value v(pson::JSON_ARRAY);
    v.AsArray().Push(new Value(pson::JSON_NULL));
    v.AsArray().Push(new Value(pson::JSON_NUMBER));
    v.AsArray().GetAsNumber(1) = 1122334455;
    EXPECT_EQ(pson::PrintValue(v), "[null,1122334455.000000]");
}

TEST(GenTest, ObjectTest)
{
    // TODO
    // Use json text , and prase to the json object, 
    // then use object to generater a json text...
}

int main() {
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
}