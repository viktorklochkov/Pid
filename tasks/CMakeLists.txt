# Create a main program using the library
add_executable(run_pid pid.cpp)
target_link_libraries(run_pid Pid ${ROOT_LIBRARIES})
target_include_directories(run_pid PUBLIC ${CMAKE_SOURCE_DIR}/src)

install(TARGETS run_pid RUNTIME DESTINATION bin)
