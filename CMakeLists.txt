cmake_minimum_required(VERSION 3.0)

project(iniparser)
add_compile_options(-std=c++11)

add_executable(test_iniparser "test.cpp" "iniparser.cpp")
add_test(test_iniparser test_iniparser)

enable_testing()
