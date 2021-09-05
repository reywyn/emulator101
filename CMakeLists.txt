cmake_minimum_required(VERSION 3.20)
project(emulator101)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(emulator101 ${SOURCES})
