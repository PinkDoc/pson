project(Json_Test)

cmake_minimum_required(VERSION 3.16)

SET(HEADS Object.hpp Value.hpp Array.hpp)

#add_executable(Value_Test Test.cc ${HEADS})
add_executable(ParseTest ParseTest.cc ${HEADS})

target_link_libraries(ParseTest
        gtest
        pthread
)