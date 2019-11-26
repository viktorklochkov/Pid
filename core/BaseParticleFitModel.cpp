//
// Created by eugene on 26/11/2019.
//

#include "BaseParticleFitModel.h"

void BaseParticleFitModel::fillParticleInfoFromDB(TDatabasePDG *pdgDB) {
    auto particleDef = pdgDB->GetParticle(pdgID_);

    if (particleDef) {
        mass_ = particleDef->Mass();
        charge_ = particleDef->Charge();
        name_ = particleDef->GetName();
    } else {
        //TODO(EK) Warning
    }
}
