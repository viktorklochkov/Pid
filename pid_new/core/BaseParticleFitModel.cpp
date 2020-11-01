//
// Created by eugene on 26/11/2019.
//

#include <RooExtendPdf.h>

#include "BaseParticleFitModel.h"
#include "FitParameter.h"

void BaseParticleFitModel::fillParticleInfoFromDB() {
    mass_ = PdgHelper::mass(pdgID_);
    charge_ = PdgHelper::charge(pdgID_);
    name_ = PdgHelper::isIon(pdgID_)? Form("ion_%d_%d", PdgHelper::getZ(pdgID_), PdgHelper::getA(pdgID_)): PdgHelper::getParticlePdg(pdgID_)->GetName();
}

FitParameter *BaseParticleFitModel::addParameter(RooRealVar *var) {
    assert(var);
    auto insertResult = parameterMap_.emplace(var->GetName(), FitParameter(var, var->GetName()));
    /* ensure parameter is not exists */
    assert(insertResult.second);
    return &insertResult.first->second;
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
    assert(!name_.empty());

    assert(observable_);
    assert(xmax_ > xmin_);
    assert(mass_ > 0);
    assert(charge_ != 0);

    initModel();
    initExtPdf();

    for (const auto& par : parameterMap_) {
        auto rooVar = par.second.getVar();
        auto &name = par.first;
        std::string newName(getParPrefix() + name);
        rooVar->SetName(newName.c_str());
    }
}

void BaseParticleFitModel::pickFitParameterResultsAt(double x) {
    for (auto &p : parameterMap_) {
        p.second.pickFitResultAt(x);
    }
}

void BaseParticleFitModel::initExtPdf() {
    auto normPar = addParameter("integral");
//    normPar->range(0, RooNumber::infinity());
    this->extPdf_.reset(new RooExtendPdf(("ext_" + getName()).c_str(), "", *getFitModel(), *normPar->getVar()));
}

FitParameter *BaseParticleFitModel::addParameter(const std::string &name) {
    auto insertResult = parameterMap_.emplace(name, FitParameter(name));
    assert(insertResult.second);
    return &insertResult.first->second;
}

void BaseParticleFitModel::saveModelTo(TDirectory *dir) const {
    assert (dir);

    auto saveDir = dir->mkdir(getName().c_str());

    for (auto &p : parameterMap_) {
        saveDir->WriteObject(p.second.toTGraph(), p.second.getName().c_str());
    }
}

size_t BaseParticleFitModel::MODEL_ID_COUNTER = 0UL;

