cmake_minimum_required(VERSION 3.15)
project(playlist)

set(CMAKE_CXX_STANDARD 17)

add_executable(playlist lib_playlist.h playlist_example.cc)
add_executable(main lib_playlist.h main.cpp)