#include <gtest/gtest.h>
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
    pson::Value v1 , v2, v3, v4;

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

TEST(ParseTest, ParseString_Right )
{
    std::string s1 = " \" Fuck You baby \"";
    pson::Value v;

    pson::Parser parser(s1);
    auto ret = parser.Parse(v);
    EXPECT_EQ(ret, true);
    EXPECT_EQ(v.AsString(), " Fuck You baby ");

}

TEST(ParseTest, ParseTest_ParseArray_Right )
{
    std::string array = " [\"hello \", null, false, \"fuck\"] ";
    pson::Value a;

    pson::Parser parser(array);
    auto ret = parser.Parse(a);
    EXPECT_EQ(ret, true);
    EXPECT_EQ(a.AsArray().Get(0).AsString(), "hello ");
    EXPECT_EQ(a.AsArray().Get(1).AsNull(), 0);
    EXPECT_EQ(a.AsArray().Get(2).AsBool(), false);
    EXPECT_EQ(a.AsArray().Get(3).AsString(), "fuck");

   /*
    *  std::string a2 = " [] ";
    parser.SetData(const_cast<char*>(a2.data()), a2.size());
    ret = parser.Parse(a);

    EXPECT_EQ(ret, true);*/
}

TEST(ParseTest, ParseTest_ParseObject_Right_Test)
{

    std::string object = "{\n"
                         "    \n"
                         "    \"version\": \"0.2.0\",\n"
                         "    \"configurations\": [\n"
                         "        {\n"
                         "            \"name\": \"g++ zxczczxc\",\n"
                         "            \"type\": \"cppdbg\",\n"
                         "            \"request\": \"launch\",\n"
                         "            \"program\": \"${fileDirname}/${fileBasenameNoExtension}\",\n"
                         "            \"args\": [],\n"
                         "            \"stopAtEntry\": false,\n"
                         "            \"cwd\": \"${fileDirname}\",\n"
                         "            \"environment\": [],\n"
                         "            \"externalConsole\": false,\n"
                         "            \"MIMode\": \"gdb\",\n"
                         "            \"setupCommands\": [\n"
                         "                {\n"
                         "                    \"description\": \"zxczxc\",\n"
                         "                    \"text\": \"-enable-pretty-printing\",\n"
                         "                    \"ignoreFailures\": true\n"
                         "                },\n"
                         "                {\n"
                         "                    \"description\": \"zxczxczxcIntel\",\n"
                         "                    \"text\": \"-gdb-set disassembly-flavor intel\",\n"
                         "                    \"ignoreFailures\": true\n"
                         "                }\n"
                         "            ],\n"
                         "            \"preLaunchTask\": \"C/C++: g+zxczxc\",\n"
                         "            \"miDebuggerPath\": \"/usr/bin/gdb\"\n"
                         "        }\n"
                         "    ]\n"
                         "}";

    pson::Parser parser(object);
    pson::Value o;
    auto ret = parser.Parse(o);

    auto& asObj = o.AsObject();

    EXPECT_EQ(ret, true);
    EXPECT_EQ(asObj.Get("version").AsString(), "0.2.0");
    EXPECT_EQ(asObj.Get("configurations").AsArray().Get(0).AsObject().Get("stopAtEntry").AsBool(), false);
    EXPECT_EQ(asObj.Get("configurations").AsArray().Get(0).AsObject().Get("setupCommands").IsArray(), true);
    auto& array = asObj.Get("configurations").AsArray().Get(0).AsObject().Get("setupCommands").AsArray();
    EXPECT_EQ(array.Get(1).AsObject().Get("text").AsString(), "-gdb-set disassembly-flavor intel");
}

int main()
{

    testing::InitGoogleTest();
    RUN_ALL_TESTS();

   // TEST();
}