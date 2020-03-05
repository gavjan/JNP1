cmake_minimum_required(VERSION 3.15)
project(insertion)

set(CMAKE_CXX_STANDARD 17)

add_executable(example1 poset.h poset.cc test/poset_example1.c)
add_executable(example2 poset.h poset.cc test/poset_example2.cc)