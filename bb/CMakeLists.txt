include_directories(${CMAKE_CURRENT_SOURCE_DIR})
ROOT_GENERATE_DICTIONARY(G__PidBB
        BetheBlochHelper.h Shine.h
        LINKDEF BBLinkDef.h
        OPTIONS -noIncludePaths
        )

add_library(pid_bb SHARED Shine.cpp BetheBlochHelper.cpp G__PidBB.cxx)
target_include_directories(pid_bb PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(pid_bb PUBLIC pid_core)
