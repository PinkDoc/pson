#include "../json_parser.hpp"
#include "../json_writer.hpp"
#include <stdio.h>
#include <cassert>
#include <iostream>

using namespace pson;

#define LOG_OUTPUT_STREAM stdout
#define LOG_LOG_TIME_FORMAT "%Y-%m-%d %H:%M:%S"

#define LOG(...) do { \
    OutPutHeader(__FILE__, __FUNCTION__);\
    fprintf(LOG_OUTPUT_STREAM, __VA_ARGS__);\
    fprintf(LOG_OUTPUT_STREAM, "\n");\
    fflush(LOG_OUTPUT_STREAM); } while(0)




void OutPutHeader(const char* filename, const char* function)
{
    auto t = time(nullptr);
    tm *curTime = localtime(&t);  // NOLINT
    char time_str[32];            
    ::strftime(time_str, 32, LOG_LOG_TIME_FORMAT, curTime);
    fprintf(LOG_OUTPUT_STREAM, "[%s]""[FileName : %s]""[Function : %s] ", time_str, filename, function);
}


std::vector<char> GetJsonData(const std::string& filename)
{
    LOG("json file name %s", filename.c_str());
    FILE* file_stream = fopen(filename.c_str(), "r+");
    std::vector<char> data;
    
    if (file_stream == nullptr) {
        LOG("fopen FAIL");
        return data;
    }
    auto cur_offset = ftell(file_stream);
    fseek(file_stream, 0, SEEK_END);
    auto size  = ftell(file_stream);
    fseek(file_stream, cur_offset, SEEK_SET);
    LOG("json data size : %ld",size);
    data.resize(size);
    auto ok = fread(data.data(), size, 1, file_stream);

    return data;
}

int main() {
    Value v;
    // example for 
    std::string filename = "example.json";
    std::vector<char> data = GetJsonData("example.json");
    Parser p(data.data(), data.size());
    bool ok = p.Parse(v);
    if (!ok) {
        LOG("parse FAIL!");
        return 0;
    }

    std::cout << "====================================" << std::endl;
    std::cout << v.print() << std::endl;


}