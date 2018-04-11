cmake_minimum_required(VERSION 3.10)
project(CryptoDIP_simple_bus)

set(CMAKE_VERBOSE_MAKEFILE off)

# Version number
set(CRYPTODIP_VERSION_MAJOR 0)
set(CRYPTODIP_VERSION_MINOR 1)

# Set the compiler
set(CMAKE_CXX_COMPILER "g++-5")

# SystemC File tree Settings
set(SYSTEMC_HOME "/usr/local/systemc-2.3.2")
set(ARCH_SUFFIX "linux64")

# SystemC flags
set(FLAGS_COMMON "-Wall")
set(FLAGS_STRICT "-pedantic -Wno-long-long")
set(FLAGS_WERROR "-Werror")

set(SYSTEMC_CXXFLAGS "${FLAGS_COMMON} ${FLAGS_STRICT} ${FLAGS_WERROR}")
set(SYSTEMC_INC_DIR "${SYSTEMC_HOME}/include")
set(SYSTEMC_LIB_DIR "${SYSTEMC_HOME}/lib-${ARCH_SUFFIX}")

# SystemC lib flags
set(SYSTEMC_LDFLAGS "-L${SYSTEMC_LIB_DIR} -Wl,-rpath=${SYSTEMC_LIB_DIR}")
set(SYSTEMC_LIBS "-lsystemc -lm")
set(LIBDIR "-L.")


# General lib Flags
set(LDFLAGS "${LDFLAGS} ${SYSTEMC_CXXFLAGS} ${LIBDIR} ${SYSTEMC_LDFLAGS}")

# CXX flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SYSTEMC_CXXFLAGS} -O2")
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} ${SYSTEMC_CXXFLAGS}")

# Linker Flags
set(CMAKE_EXE_LINKER_FLAGS "${LIBDIR} ${SYSTEMC_LDFLAGS} ${SYSTEMC_LIBS}")


# OpenCV
find_package(OpenCV REQUIRED)

# SystemC include dir
include_directories("${SYSTEMC_INC_DIR}")
include_directories(".")

add_executable(CryptoDIP.x
        simple_bus.cpp
        simple_bus_arbiter.cpp
        simple_bus_main.cpp
        simple_bus_master_blocking.cpp
        simple_bus_master_direct.cpp
        simple_bus_master_non_blocking.cpp
        simple_bus_communication.cpp
        simple_bus_gpu.cpp
        simple_bus_tools.cpp
        simple_bus_types.cpp)

target_link_libraries(CryptoDIP.x ${SYSTEMC_LIB_DIR}/libsystemc.a ${OpenCV_LIBS})
