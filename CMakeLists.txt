cmake_minimum_required(VERSION 3.26)
project(thread_pool)

set(CMAKE_CXX_STANDARD 17)

add_executable(thread_pool main.cpp
        threadpool.cpp
        threadpool.h
        ptr_fun.cpp
        ptr_fun.h)
