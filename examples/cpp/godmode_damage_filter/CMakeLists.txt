cmake_minimum_required(VERSION 3.10)
project(godmode_damage_filter CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(../../../include)

add_library(godmode_damage_filter SHARED src/filter_main.cpp)
set_target_properties(godmode_damage_filter PROPERTIES PREFIX "lib" SUFFIX ".so")
