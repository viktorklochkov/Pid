

add_library(pid_core SHARED BaseParticleFitModel.cpp BaseFitterHelper.cpp PdgHelper.cpp PdgHelper.h FitParameterConstraint.cpp FitParameterConstraint.h FitParameterTrack.cpp FitParameterTrack.h FitParameter.cpp FitParameter.h)
target_link_libraries(pid_core PUBLIC ROOT::RooFit)
target_include_directories(pid_core PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)