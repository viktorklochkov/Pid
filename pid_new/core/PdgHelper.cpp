//
// Created by eugene on 27/11/2019.
//

#include "PdgHelper.h"

double PdgHelper::mass(Pdg_t pdg) {
  if (isIon(pdg)) {
    return 0.938 * std::get<0>(getAZIon(pdg));
  } else if (getParticlePdg(pdg)) {
    return getParticlePdg(pdg)->Mass();
  }

  throw std::runtime_error("Unknown pdg code");
}

double PdgHelper::charge(Pdg_t pdg) {
  if (isIon(pdg)) {
    return 0.938 * std::get<1>(getAZIon(pdg));
  } else if (getParticlePdg(pdg)) {
    return getParticlePdg(pdg)->Charge();
  }

  throw std::runtime_error("Unknown pdg code");
}

std::tuple<int, int> PdgHelper::getAZIon(Pdg_t pdg) {
  assert(pdg > 1000000000);
  int A = (pdg % 10000) / 10;
  int Z = (pdg % 10000000) / 10000;
  return std::tuple<int, int>(A, Z);
}

int PdgHelper::getZ(Pdg_t pdg) {
  return std::get<1>(getAZIon(pdg));
}

int PdgHelper::getA(Pdg_t pdg) {
  return std::get<0>(getAZIon(pdg));
}

bool PdgHelper::isIon(Pdg_t pdg) {
  return pdg > 1000000000;
}

TParticlePDG* PdgHelper::getParticlePdg(Pdg_t pdg) {
  return TDatabasePDG::Instance()->GetParticle(pdg);
}
