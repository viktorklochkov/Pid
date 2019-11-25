


add_library(pid_bb_static Shine.cpp)
target_include_directories(pid_bb_static PUBLIC ${CMAKE_SOURCE_DIR})

add_library(pid_bb SHARED Shine.cpp)
target_include_directories(pid_bb PUBLIC ${CMAKE_SOURCE_DIR})
