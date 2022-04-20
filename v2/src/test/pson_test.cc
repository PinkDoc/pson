#include <gtest/gtest.h>

#include "src/json_writer.hpp"
#include "src/json_parser.hpp"
#include "src/json_value.hpp"


TEST(ValueTest, move)
{
    using namespace pson;
    Value varray(JSON_ARRAY);
    Value vbool(JSON_BOOL);
    varray = std::move(vbool);

    EXPECT_EQ(varray.is_bool(), true);
    EXPECT_EQ(vbool.is_null(), true);
}

TEST(ValueTest, copy)
{
    using namespace pson;
    Value varray(JSON_ARRAY);
    Value vbool(JSON_BOOL);
    varray = vbool;

    Value cp_bool(vbool);

    EXPECT_EQ(varray.is_bool(), true);
    EXPECT_EQ(vbool.is_bool(), true);
    EXPECT_EQ(cp_bool.is_bool(), true);
}

TEST(ValueTest, reset_as_and_is)
{
    using namespace pson;

    Value v;
    EXPECT_EQ(v.is<Null>(), true);
    v.reset_as<Bool>();
    EXPECT_EQ(v.is<Bool>(), true);
    v.reset_as<Number>();
    EXPECT_EQ(v.is<Number>(), true);
    v.reset_as<Array>();
    EXPECT_EQ(v.is<Array>(), true);
    v.reset_as<Object>();
    EXPECT_EQ(v.is<Object>(), true);
}

TEST(ValueTest, swap_test)
{
    using namespace pson;

    Value v1, v2;
    v1.reset_as<Object>();
    v2.reset_as<Array>();
    v1.swap(v2);
    EXPECT_EQ(v1.is<Array>(), true);
    EXPECT_EQ(v2.is<Object>(), true);
}

TEST(ObjectTest, oper_test)
{
    using namespace  pson;
    Value o(JSON_OBJECT);
    Value o2(JSON_OBJECT);

    // insert, size, operator[], erase, at, as
    // insert
    o.insert("hello world", Value(JSON_ARRAY));
    o.insert("my null", Value(JSON_NULL));
    o.insert("my string", Value(JSON_STRING));
    o["my string"].as<String>() = "I am String";

    EXPECT_EQ(o.size(), 3);
    EXPECT_EQ(o["hello world"].size(), 0);
    EXPECT_EQ(o["my null"].is<Null>(), true);

    o.erase("my null");

    EXPECT_EQ(o.size(), 2);
    EXPECT_EQ(o.at("my string").as<String>(), "I am String");

    // basic copy and move
    o2 = o;

    EXPECT_EQ(o.at("my string").as<String>(), "I am String");

    Value o3(std::move(o2));

    EXPECT_EQ(o3.size() > 0, true);
    EXPECT_EQ(o3["my string"].as<String>(), "I am String");
    EXPECT_EQ(o2.is<Null>(), true);
}

TEST(ParserTest, parse)
{
    using namespace pson;

    std::string json = "{\n"
                       "  \"pink\" : {\n"
                       "    \"ktouch\" : \"phone\",\n"
                       "    \"xiaomi\" : \"phone\"\n"
                       "  },\n"
                       "  \"computer\" : [\"惠普\", 100.0, false, true],\n"
                       "  \"age\" : 20,\n"
                       "  \"best code\" : \"non\",\n"
                       "  \"man\" : true\n"
                       "}";

    Value v;
    v.parse(json);

    EXPECT_EQ(v.is<Object>(), true);
    EXPECT_EQ(v["computer"][0].as<String>(), "惠普");
    EXPECT_EQ(v["pink"]["ktouch"].as_string(), "phone");
    EXPECT_EQ(v["computer"][1].as_number(), 100.0);
    EXPECT_EQ(v["man"].as<Bool>(), true);


}

int main()
{
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
}