//
// Created by eugene on 26/11/2019.
//

#include "BetheBlochHelper.h"

#include <TDatabasePDG.h>
#include <TParticlePDG.h>

BetheBlochFunc_t BetheBlochHelper::makeBBForMass(double mass, BetheBlochFunc_t bb_src) {
    return [=](double momentum) { return bb_src(momentum / mass); };
}

BetheBlochFunc_t BetheBlochHelper::makeBBForPdg(Pdg_t pdg, BetheBlochFunc_t bb_src) {
    assert(PdgHelper::charge(pdg) == 1.);
    double mass = PdgHelper::mass(pdg);
    return [=](double momentum) { return bb_src(momentum / mass); };
}
