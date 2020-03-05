cmake_minimum_required(VERSION 3.15)
project(fibin)

set(CMAKE_CXX_STANDARD 17)

add_executable(example fibin.h fibin_example.cc)
add_executable(test fibin.h fibin_test.cc)