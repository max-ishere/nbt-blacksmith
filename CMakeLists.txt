cmake_minimum_required(VERSION 3.10)
project(nbt-blacksmith)

add_library(nbt-blacksmith OBJECT nbt.cpp sbin.cpp ios-bin.cpp)

add_executable(main main.cpp)
target_link_libraries(main nbt-blacksmith)

add_executable(test test.cpp)
target_link_libraries(test nbt-blacksmith)

add_executable(region-tool region-tool.cpp)
target_link_libraries(region-tool nbt-blacksmith)
