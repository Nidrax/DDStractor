cmake_minimum_required(VERSION 3.28)
project(DDStractor)

set(CMAKE_CXX_STANDARD 20)

add_executable(DDStractor
        src/dds_file.cpp
        src/dds_printer.cpp
        src/main.cpp
)
target_include_directories(DDStractor PRIVATE
        lib
)
