
set(SOURCES
    Getter.cpp
    CutGGetter.cpp
    ParticlePdf1D.cpp
    ParticlePdf2D.cpp )

string(REPLACE ".cpp" ".h" HEADERS "${SOURCES}")

set(HEADERS ${HEADERS} Constants.h )

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})

set(DICT_FILE_NAME G__${PROJECT_NAME})
set(PCM_FILE_NAME lib${PROJECT_NAME})

ROOT_GENERATE_DICTIONARY(${DICT_FILE_NAME} ${HEADERS} LINKDEF PidLinkDef.h)
add_library(Pid SHARED ${SOURCES} ${DICT_FILE_NAME})
target_link_libraries(Pid ${ROOT_LIBRARIES})
