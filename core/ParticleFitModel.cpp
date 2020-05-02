//
// Created by eugene on 01/05/2020.
//

#include "ParticleFitModel.h"

ParticleFitModel::ParticleFitModel(const std::string &name, RooAbsPdf &pdf, RooAbsData &data,
                                   const std::string &par_prefix,
                                   const std::string &par_suffix) {
    pdf_ = &pdf;

    name_ = name;
    par_prefix_ = par_prefix;
    par_suffix_ = par_suffix;

    /* lookup params, observables */
    auto params = pdf_->getParameters(data);

    for (size_t i = 0; i < params->size(); ++i) {
        auto pVar = dynamic_cast<RooRealVar *>(params->operator[](i));

        if (pVar == nullptr) {
            std::cerr << "Parameter is not RooRealVar" << std::endl;
        } else {

            std::string varName{pVar->namePtr()->GetName()};

            if (pVar->isDerived()) {
                std::cerr << "Parameter is derived" << std::endl;
            } else {

                if (FitParameter::par_find(pVar)) {
                    std::cerr << "Parameter is already exists" << std::endl;
                } else {
                    std::cout << "Adding parameter " << varName << " to the global registry" << std::endl;
                    FitParameter::add_parameter(*pVar);
                }

                if (varName.substr(0, par_prefix.size()) == par_prefix) {
                    std::cout << "Adding parameter " << varName << " to the model"  << std::endl;
                    parameters_.push_back(FitParameter::par_find(pVar));
                } else {
                    std::cerr << "Parameter '" << varName << "' has no required prefix '" << par_prefix << "'."
                              << "It won't be associated with model '" << name_ << "'" << std::endl;
                }
            }



            }
        } // params

    }

