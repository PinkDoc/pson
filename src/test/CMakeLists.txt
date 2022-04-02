project(Json_Test)

cmake_minimum_required(VERSION 3.16)

SET(HEADS ../Value.hpp ../Parser.hpp ../Generater.hpp)

#add_executable(Value_Test Test.cc ${HEADS})
add_executable(ParseTest ParseTest.cc ${HEADS})
add_executable(GeneraterTest GeneraterTest.cc ${HEADS})

target_link_libraries(ParseTest
        gtest
        pthread
)

target_link_libraries(GeneraterTest
        gtest
        pthread
)