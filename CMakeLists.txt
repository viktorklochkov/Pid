cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Pid)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_cpp_STANDARD 11)
set(PROJECT_VERSION 1.0)

# by default build optimized code with debug symbols
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
endif ()

# in DEBUG mode make verbose Makefile
if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif ()

message(STATUS "Using CXX flags for ${CMAKE_BUILD_TYPE}: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")

set(SOURCES
        src/Fitter.cpp
        src/ParticleFit.cpp
        src/Parameters.cpp
        src/Getter.cpp
#        src/Container.cpp
        )

string(REPLACE ".cpp" ".h" HEADERS "${SOURCES}")

set(HEADERS
        ${HEADERS}
        src/Constants.h
        )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

# Telling CMake where to find the ROOT installation.
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/lib")

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS Core MathCore RIO Hist Tree)

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
add_definitions(${ROOT_CXX_FLAGS})

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})

set(DICT_FILE_NAME G__${PROJECT_NAME})
set(PCM_FILE_NAME lib${PROJECT_NAME})

ROOT_GENERATE_DICTIONARY(${DICT_FILE_NAME} ${HEADERS} LINKDEF src/PidLinkDef.h)
add_library(Pid SHARED ${SOURCES} ${DICT_FILE_NAME})
target_link_libraries(Pid ${ROOT_LIBRARIES})

# Create a main program using the library
add_executable(main main.cpp)
target_link_libraries(main Pid ${ROOT_LIBRARIES})
target_include_directories(main PUBLIC src)

# Create a main program using the library
add_executable(pid macro/pid.cpp)
target_link_libraries(pid Pid ${ROOT_LIBRARIES})
target_include_directories(pid PUBLIC src)

# Packaging routine
# For complete explanation
# @see https://cmake.org/cmake/help/git-master/manual/cmake-packages.7.html#creating-packages
include(GenerateExportHeader)

generate_export_header(Pid)
set_property(TARGET Pid PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET Pid PROPERTY SOVERSION ${PROJECT_VERSION})
set_property(TARGET Pid PROPERTY INTERFACE_Pid_MAJOR_VERSION 1)
set_property(TARGET Pid APPEND PROPERTY COMPATIBLE_INTERFACE_STRING INTERFACE_Pid_MAJOR_VERSION)

install(TARGETS Pid EXPORT PidTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

install(
        FILES
        ${HEADERS}
        "${CMAKE_CURRENT_BINARY_DIR}/pid_export.h"
        DESTINATION
        include/pid
        COMPONENT
        Devel
)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}_rdict.pcm"
        "${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}.rootmap"
        DESTINATION
        lib
        OPTIONAL
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/Pid/PidConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

export(EXPORT PidTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/Pid/PidTargets.cmake")

set(ConfigPackageLocation lib/cmake/Pid)

set(Pid_INCLUDE_DIR "include")
set(Pid_LIBRARY_DIR "lib")

configure_package_config_file(
        PidConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/Pid/PidConfig.cmake
        INSTALL_DESTINATION ${ConfigPackageLocation}
        PATH_VARS Pid_INCLUDE_DIR Pid_LIBRARY_DIR
        #  [NO_SET_AND_CHECK_MACRO]
        #  [NO_CHECK_REQUIRED_COMPONENTS_MACRO]
        #  [INSTALL_PREFIX <path>]
)

install(EXPORT PidTargets
        FILE
        PidTargets.cmake
        DESTINATION
        ${ConfigPackageLocation}
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/Pid/PidConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/Pid/PidConfigVersion.cmake"
        DESTINATION
        ${ConfigPackageLocation}
        COMPONENT
        Devel
        )

install (TARGETS pid RUNTIME DESTINATION bin)
add_subdirectory(core)
add_subdirectory(bb)
add_subdirectory(model)
add_subdirectory(examples/shine)

