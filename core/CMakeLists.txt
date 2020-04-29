

add_library(pid_core SHARED BaseParticleFitModel.cpp FitterHelper.cpp PdgHelper.cpp PdgHelper.h FitParameter.cpp FitParameter.h)
target_link_libraries(pid_core PUBLIC ROOT::RooFit)
target_include_directories(pid_core PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)
set_target_properties(pid_core PROPERTIES CXX_STANDARD 14)