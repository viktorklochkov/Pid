//
// Created by eugene on 26/11/2019.
//

#include "BetheBlochHelper.h"

#include <TDatabasePDG.h>
#include <TParticlePDG.h>

BetheBlochFunc_t BetheBlochHelper::makeBBForMass(double mass, BetheBlochFunc_t bb_src) {
  return [=] (double momentum) { return bb_src(momentum/mass); };
}

BetheBlochFunc_t BetheBlochHelper::makeBBForPdg(int pdg, BetheBlochFunc_t bb_src) {
  auto pdgDB = new TDatabasePDG;

  double mass {-1.};

  auto particleDef = pdgDB->GetParticle(pdg);
  if (particleDef) {
    mass = particleDef->Mass();
    if (fabs(particleDef->Charge()) != 1.) {
      throw std::runtime_error("BB function is only tabulated for charge == 1");
    }
  } else if (pdg > 1000000000) {
    /* 100ZZZAAA0 */
    int A = (pdg % 10000)/10;
    int Z = (pdg % 10000000)/10000;
    mass = A*0.938;
    if (Z != 1) {
      throw std::runtime_error("BB function is only tabulated for charge == 1");
    }
  }

  return [=] (double momentum) { return bb_src(momentum/mass); };
}
