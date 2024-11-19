# cmakeのversion指定
cmake_minimum_required(VERSION 3.5)

# compilerの指定
set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")

# project名の設定
project(cpp_lib)

# file名 .cppを収集する
file(GLOB_RECURSE SOURCES "./src/*.cpp")

# cppが増えてきたら外す。
# projectに.cpp filesをつける
add_library(cpp_lib STATIC ${SOURCES})

# projectに.hppをつける
target_include_directories(cpp_lib INTERFACE ./include)

# math.hが使えるようにする
target_link_libraries(cpp_lib m)
