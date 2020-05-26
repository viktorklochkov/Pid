include_directories(${CMAKE_CURRENT_SOURCE_DIR})


ROOT_GENERATE_DICTIONARY(G__BetheBlochShine BetheBlochShine.h
        LINKDEF BetheBlochShineLinkDef.h
        OPTIONS -inlineInputHeader)
add_library(pid_bb_shine SHARED BetheBlochShine.cpp G__BetheBlochShine.cxx)


add_library(pid_bb SHARED BetheBlochShine.cpp BetheBlochHelper.cpp)
target_include_directories(pid_bb PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(pid_bb PUBLIC pid_core)
