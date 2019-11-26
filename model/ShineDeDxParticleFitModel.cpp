//
// Created by eugene on 26/11/2019.
//

#include "ShineDeDxParticleFitModel.h"
#include "AsymmetricGaussianPDF.h"

#include <RooRealVar.h>

ShineDeDxParticleFitModel::ShineDeDxParticleFitModel(int pdgId) : BaseParticleFitModel(pdgId) {}


RooAbsPdf *ShineDeDxParticleFitModel::getFitModel() {
    return pdf_;
}

std::vector<RooAbsReal *> ShineDeDxParticleFitModel::getFitParams() {
    return {bb, sigma, d};
}

void ShineDeDxParticleFitModel::initModel() {
    bb = new RooRealVar();
    sigma = new RooRealVar();
    d = new RooRealVar();

    pdf_ = new AsymmetricGaussianPDF(getName().c_str(), "", *getObservable(), *bb, *sigma, *d);
}
