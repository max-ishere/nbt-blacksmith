find_package(PkgConfig)
pkg_search_module(GTEST REQUIRED gtest_main)

add_executable(nbt-test test.cpp
  sbin.cpp
  primitive.cpp
  array.cpp)

target_link_libraries(nbt-test
  PUBLIC nbt-blacksmith
  PUBLIC ${GTEST_LDFLAGS})

target_compile_options(nbt-test PUBLIC ${GTEST_CXXFLAGS})

set_target_properties(nbt-test PROPERTIES
  CXX_STANDARD 17)

add_custom_command(TARGET nbt-test
  POST_BUILD  
  COMMAND nbt-test
  COMMENT "Running gtest on build")
