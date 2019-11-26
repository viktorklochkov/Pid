//
// Created by eugene on 26/11/2019.
//

#include "BaseParticleFitModel.h"

void BaseParticleFitModel::fillParticleInfoFromDB() {
    mass_ = PdgHelper::mass(pdgID_);
    charge_ = PdgHelper::charge(pdgID_);
    name_ = PdgHelper::isIon(pdgID_)? Form("ion_%d_%d", PdgHelper::getZ(pdgID_), PdgHelper::getA(pdgID_)): PdgHelper::getParticlePdg(pdgID_)->GetName();
}
