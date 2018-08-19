cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(pid)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_cpp_STANDARD 14)
set(CMAKE_BUILD_TYPE debug)

set(SOURCES 
    src/Fitter.cpp
    src/ParticleFit.cpp
    src/Parameters.cpp
    src/Getter.cpp
    )

string(REPLACE ".cpp" ".h" HEADERS "${SOURCES}")

set(HEADERS 
    ${HEADERS}
    src/Constants.h
    )

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Telling CMake where to find the ROOT installation.
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/lib")

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS Core MathCore RIO Hist Tree)

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
add_definitions(${ROOT_CXX_FLAGS})

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})

ROOT_GENERATE_DICTIONARY(G__Pid ${HEADERS} LINKDEF src/PidLinkDef.h)
add_library(Pid SHARED ${SOURCES} G__Pid)
target_link_libraries(Pid ${ROOT_LIBRARIES})

# Create a main program using the library
add_executable(main main.cpp)
target_link_libraries(main Pid ${ROOT_LIBRARIES})
target_include_directories(main PUBLIC src)
