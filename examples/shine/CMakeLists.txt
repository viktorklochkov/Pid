add_executable(RunFitShine RunFitShine.cpp)
target_link_libraries(RunFitShine pid_core pid_bb pid_model)

add_executable(RunFitShine2 RunFitShine2.cpp)
target_link_libraries(RunFitShine2 pid_core pid_bb pid_model ${ROOT_LIBRARIES} ROOTVecOps)
