add_executable(RunFitShine RunFitShine.cpp)
target_link_libraries(RunFitShine pid_core pid_bb pid_model)

add_executable(FitBinnedTracks FitBinnedTracks.cpp)
target_link_libraries(FitBinnedTracks pid_core pid_bb pid_model ${ROOT_LIBRARIES} ROOTVecOps)

add_executable(FitResultsExplorer FitResultsExplorer.cpp)
target_link_libraries(FitResultsExplorer pid_core pid_bb pid_model ${ROOT_LIBRARIES} ROOTVecOps)
