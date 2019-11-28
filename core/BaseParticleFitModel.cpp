//
// Created by eugene on 26/11/2019.
//

#include "BaseParticleFitModel.h"

void BaseParticleFitModel::fillParticleInfoFromDB() {
    mass_ = PdgHelper::mass(pdgID_);
    charge_ = PdgHelper::charge(pdgID_);
    name_ = PdgHelper::isIon(pdgID_)? Form("ion_%d_%d", PdgHelper::getZ(pdgID_), PdgHelper::getA(pdgID_)): PdgHelper::getParticlePdg(pdgID_)->GetName();
}

RooRealVar *BaseParticleFitModel::addParameter(RooRealVar *var) {
    assert(var);
    parameterMap_.emplace(var->GetName(), FitParameter(var, var->GetName()));
    return var;
}

void BaseParticleFitModel::applyParameterConstraintsAt(double x) {
    for (auto &p : parameterMap_) {
        p.second.applyConstraint(x);
    }
}

void BaseParticleFitModel::print() {
    std::ostringstream pm;
    pm << name_ << ":" << std::endl;
    pm << "fitting model: ";
    getFitModel()->printClassName(pm);
    pm << std::endl;
    pm << "observable: ";
    getObservable()->printName(pm);
    pm << std::endl;

    std::cout << pm.str();
}

void BaseParticleFitModel::initialize() {
    assert(name_ != "");

    assert(observable_);
    assert(xmax_ > xmin_);
    assert(mass_ > 0);
    assert(charge_ != 0);

    initModel();

    for (auto par : getFitParams()) {
        std::string newName(getParPrefix() + std::string(par->GetName()));
        par->SetName(newName.c_str());
    }
}
