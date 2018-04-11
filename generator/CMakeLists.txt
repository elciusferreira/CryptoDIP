cmake_minimum_required(VERSION 3.10)

set(CMAKE_VERBOSE_MAKEFILE off)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER g++-5)

# OpenCV
find_package(OpenCV REQUIRED)
find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_CXX_STANDARD_LIBRARIES "-lssl -lcrypto")

include_directories(".")

add_executable(generator generator.cpp)

target_link_libraries(generator ${OpenCV_LIBS})
