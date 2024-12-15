cmake_minimum_required(VERSION 3.30)
project(Advent_of_Code_2024)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)


add_library(util STATIC)
target_sources(util PUBLIC
        FILE_SET modules
        TYPE CXX_MODULES
        FILES libutil/Util.ixx
        libutil/Grid.ixx
        libutil/Misc.ixx
)

add_executable(day-1 day-1/main.cpp)

add_executable(day-2 day-2/main.cpp)

add_executable(day-3 day-3/main.cpp)

add_executable(day-4 day-4/main.cpp)
target_link_libraries(day-4 util)

add_executable(day-5 day-5/main.cpp)
target_link_libraries(day-5 util)

add_executable(day-6 day-6/main.cpp)
target_link_libraries(day-6 util)

add_executable(day-7 day-7/main.cpp)
target_link_libraries(day-7 util)

add_executable(day-8 day-8/main.cpp)
target_link_libraries(day-8 util)

add_executable(day-9 day-9/main.cpp)
target_link_libraries(day-9 util)
