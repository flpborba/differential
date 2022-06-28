cmake_minimum_required(VERSION 3.16)

project(ddt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_RELEASE_INIT "-march=native")

add_subdirectory(src)
add_subdirectory(tests EXCLUDE_FROM_ALL)
