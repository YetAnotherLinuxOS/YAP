cmake_minimum_required(VERSION 3.4)
project(yap)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_COMPILER "clang++")
aux_source_directory("src" SOURCES)

add_executable(yap ${SOURCES})

target_link_libraries(yap stdc++fs)

target_include_directories(yap PRIVATE "${PROJECT_SOURCE_DIR}/include")