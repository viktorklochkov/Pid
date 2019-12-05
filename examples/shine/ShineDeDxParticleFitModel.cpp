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

void ShineDeDxParticleFitModel::initModel() {
    bb = addParameter(new RooRealVar("bb", "", 0., 4., "MIP"))->getVar();
    sigma = addParameter(new RooRealVar("sigma", "", 1e-2, 5, "MIP"))->getVar();
    d = addParameter(new RooRealVar("d", "", -0.2, 0.2, "-"))->getVar();

    pdf_ = new AsymmetricGaussianPDF(getName().c_str(), "", *getObservable(), *bb, *sigma, *d);
}

