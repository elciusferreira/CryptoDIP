cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

# OpenCV
find_package(OpenCV REQUIRED)

include_directories(".")

add_executable(generator generator.cpp)

target_link_libraries(generator ${OpenCV_LIBS})
