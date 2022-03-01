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
    pson::Value* v = new pson::Value();
    val.AsArray().Push(v);
    EXPECT_EQ(val.AsArray().Size(), 1);
}

TEST(VALTEST, MoveStringTest)
{
    pson::Value v(pson::JSON_STRING);
    pson::Value ar;
    pson::String& s = v.AsString();
    s += "hello world";
    ar.ImportString(std::move(v.AsString()));
    EXPECT_EQ(s, "");
    EXPECT_EQ(ar.AsString(), "hello world");

    pson::String  ss = "Hello baby";
    ar.ImportString(std::move(ss));
    EXPECT_EQ(ar.AsString(), "Hello baby");
    EXPECT_EQ(ss, "");
}

TEST(VALTEST, MoveArrayTest)
{
   pson::Value* array = new pson::Value(pson::JSON_ARRAY);


    for(int i = 0; i < 20000; ++i)
    {
        pson::Value *str = new pson::Value(pson::JSON_STRING);
        str->AsString() += "hello world";
        array->AsArray().Push(str);
    }

    for(int i = 0; i < 20000; ++i)
    {
        EXPECT_EQ(array->AsArray().Get(i).AsString(), "hello world");
    }
    EXPECT_EQ(array->AsArray().Size(), 20000);

    delete array;
}

TEST(VALTEST, MoveObjectTest)
{
    pson::Value obj(pson::JSON_OBJECT);
    auto& o = obj.AsObject();
    for(int i = 0; i < 20000; ++i)
    {
        pson::Value *str = new pson::Value(pson::JSON_STRING);
        str->AsString() += "hello world";
        str->AsString() += std::to_string(i);
        std::string name = std::to_string(i);
       if (i % 2 == 1) o.Insert(std::move(name), str);
       else o.Insert(name, str);
    }

    for(int i = 0; i < 20000 ; ++i)
    {
        EXPECT_EQ(o.Get(std::to_string(i)).AsString(), "hello world" + std::to_string(i) );
    }

}

TEST(VALTEST, StackArraryImportTest)
{
    pson::Array array;
    pson::Value v;
    v.ImportArray(array);

}

int main() {
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
}