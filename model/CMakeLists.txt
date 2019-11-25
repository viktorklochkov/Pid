include_directories(${CMAKE_CURRENT_SOURCE_DIR})
ROOT_GENERATE_DICTIONARY(G__PidModel
        AsymmetricGaussianPDF.h
        LINKDEF PidModelLinkDef.h
        )

add_library(pid_model SHARED AsymmetricGaussianPDF.cpp DeDxShineFitModel.cpp G__PidModel.cxx)
target_include_directories(pid_model PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(pid_model
        PUBLIC
            ROOT::RooFit
            ROOT::EG
        )

