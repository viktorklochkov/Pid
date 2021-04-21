include(AnalysisTree)

set(SOURCES
  PidFiller.cxx
  )

string(REPLACE ".cxx" ".h" HEADERS "${SOURCES}")
add_library(PidAT SHARED ${SOURCES} G__PidAT.cxx)

message(${PROJECT_INCLUDE_DIRECTORIES})

include_directories(${PROJECT_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
ROOT_GENERATE_DICTIONARY(G__PidAT ${HEADERS} LINKDEF PidATLinkDef.h)
target_link_libraries(PidAT ${ROOT_LIBRARIES} AnalysisTreeBase AnalysisTreeInfra Pid)

#install(TARGETS PidAT EXPORT AnalysisTreeFillerTargets
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib
#        RUNTIME DESTINATION bin
#        INCLUDES DESTINATION include
#        )
#
#install(
#        FILES
#        ${HEADERS}
#        DESTINATION
#        include
#        COMPONENT
#        Devel
#)
#
#set(PCM_FILE_NAME libAnalysisTreeFiller)
#
#install(
#        FILES
#        "${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}_rdict.pcm"
#        DESTINATION
#        lib
#        OPTIONAL
#)
