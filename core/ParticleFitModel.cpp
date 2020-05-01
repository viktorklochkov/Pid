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
        auto p = dynamic_cast<RooRealVar*>(params->operator[](i));

        if (p && !p->isDerived()) {
            auto parName = std::string(p->namePtr()->GetName());
            parameters_.emplace_back(p, parName);
        }
    } // params

    stripParNames();

    std::cout << "Model: " << name_ << std::endl;
    std::for_each(par_begin(), par_end(), [this] (FitParameter &p) {
        std::cout << "\tParameter: " << p.getName() << std::endl;
    });

}

void ParticleFitModel::stripParNames() {
    std::for_each(par_begin(), par_end(), [this] (FitParameter &p) {
        auto name = p.getName();

        /* strip prefix */
        if (!par_prefix_.empty()) {
            if (name.substr(0, par_prefix_.size()) == par_prefix_) {
                name = name.substr(par_prefix_.size(), name.size());
            }
        }

        /* strip suffix */
        if (!par_suffix_.empty()) {
            if (name.substr(name.size() - par_suffix_.size(), name.size()) == par_suffix_) {
                name = name.substr(0, name.size() - par_suffix_.size());
            }
        }

        p.setName(name);
    });
}
