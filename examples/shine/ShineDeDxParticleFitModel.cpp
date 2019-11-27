//
// Created by eugene on 26/11/2019.
//

#include "ShineDeDxParticleFitModel.h"
#include <model/AsymmetricGaussianPDF.h>

#include <RooRealVar.h>

ShineDeDxParticleFitModel::ShineDeDxParticleFitModel(int pdgId) : BaseParticleFitModel(pdgId) {}


RooAbsPdf *ShineDeDxParticleFitModel::getFitModel() {
    return pdf_;
}

std::vector<RooRealVar *> ShineDeDxParticleFitModel::getFitParams() {
    return {bb, sigma, d};
}

void ShineDeDxParticleFitModel::initModel() {
    bb = addParameter(new RooRealVar("bb", "", 0., 4., "MIP"));
    sigma = addParameter(new RooRealVar("sigma", "", 1e-2, 5, "MIP"));
    d = addParameter(new RooRealVar("d", "", -0.2, 0.2, "-"));

    pdf_ = new AsymmetricGaussianPDF(getName().c_str(), "", *getObservable(), *bb, *sigma, *d);
}

