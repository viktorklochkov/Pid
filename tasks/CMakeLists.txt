
# Create a main program using the library
add_executable(run_pid run_pid.cpp)
target_link_libraries(run_pid Pid ${ROOT_LIBRARIES})
target_include_directories(run_pid PUBLIC ${CMAKE_SOURCE_DIR}/src)

install(TARGETS run_pid RUNTIME DESTINATION bin)

include(AnalysisTree)

add_executable(fill_at_pid fill_at_pid.cpp)
target_link_libraries(fill_at_pid Pid PidAT AnalysisTreeBase AnalysisTreeInfra ${ROOT_LIBRARIES})
target_include_directories(fill_at_pid PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/at_interface ${PROJECT_INCLUDE_DIRECTORIES})

install(TARGETS fill_at_pid RUNTIME DESTINATION bin)
