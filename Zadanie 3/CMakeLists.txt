cmake_minimum_required(VERSION 3.15)
project(fibo)

set(CMAKE_CXX_STANDARD 17)

add_executable(fibo fibo.h fibo.cc fibo_test.cpp)