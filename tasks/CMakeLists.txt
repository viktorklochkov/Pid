include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(fit_tof fit_tof.cpp)
add_dependencies(fit_tof Pid)
target_link_libraries(fit_tof Pid ${ROOT_LIBRARIES} RooFitCore RooFit RooStats)

install (TARGETS fit_tof RUNTIME DESTINATION bin)
