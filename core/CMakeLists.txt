

add_library(pid_core SHARED BaseParticleFitModel.cpp BaseFitterHelper.cpp)
target_link_libraries(pid_core PUBLIC ROOT::RooFit)
target_include_directories(pid_core PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)