cmake_minimum_required(VERSION 3.30)
project(Advent_of_Code_2024)

set(CMAKE_CXX_STANDARD 23)

# this is heuristically generated, and may not be correct
#find_package(absl CONFIG REQUIRED)

add_executable(day-1 day-1/main.cpp)

add_executable(day-2 day-2/main.cpp)

add_executable(day-3 day-3/main.cpp)

add_executable(day-4 day-4/main.cpp)
target_include_directories(day-4 PRIVATE ./include)

add_executable(day-5 day-5/main.cpp)
target_include_directories(day-5 PRIVATE ./include)

# note: 186 additional targets are not displayed.
#target_link_libraries(day-1 PRIVATE absl::strings)
