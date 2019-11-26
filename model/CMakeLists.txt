include_directories(${CMAKE_CURRENT_SOURCE_DIR})
ROOT_GENERATE_DICTIONARY(G__PidModel
        AsymmetricGaussianPDF.h
        LINKDEF PidModelLinkDef.h
        OPTIONS -noIncludePaths
        )

add_library(pid_model SHARED AsymmetricGaussianPDF.cpp G__PidModel.cxx ../core/BaseParticleFitModel.cpp ../core/BaseParticleFitModel.h ShineDeDxParticleFitModel.cpp ShineDeDxParticleFitModel.h)
target_include_directories(pid_model
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        )
target_link_libraries(pid_model
        PUBLIC
            pid_core
            ROOT::RooFit
            ROOT::EG
        )

