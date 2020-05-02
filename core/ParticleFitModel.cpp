//
// Created by eugene on 01/05/2020.
//

#include "ParticleFitModel.h"

#include <utility>

ParticleFitModel::ParticleFitModel(std::string name, RooAbsPdf &pdf, RooAbsData &data,
                                   std::string par_prefix,
                                   std::string par_suffix) {
    pdf_ = &pdf;

    name_ = std::move(name);
    par_prefix_ = std::move(par_prefix);
    par_suffix_ = std::move(par_suffix);


    FitParameterRegistry::instance().add_parameters_from_pdf(pdf, data);

}

