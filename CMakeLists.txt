cmake_minimum_required(VERSION 3.10)
project(nbt-blacksmith)


add_compile_options(
  "-std=c++2a"
  "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
  "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
  )

include_directories(include)
add_subdirectory(src)
add_subdirectory(tests)

add_executable(main main.cpp)
target_link_libraries(main nbt-blacksmith)
