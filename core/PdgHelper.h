//
// Created by eugene on 27/11/2019.
//

#ifndef PID_PDGHELPER_H
#define PID_PDGHELPER_H

#include <tuple>
#include <cassert>
#include <TDatabasePDG.h>

typedef int Pdg_t;

class PdgHelper {
public:
    static double mass(Pdg_t pdg);

    static double charge(Pdg_t pdg);

    static std::tuple<int, int> getAZIon(Pdg_t pdg);

    static int getZ(Pdg_t pdg);

    static int getA(Pdg_t pdg);

    static bool isIon(Pdg_t pdg);

    static TParticlePDG* getParticlePdg(Pdg_t pdg);

};


#endif //PID_PDGHELPER_H
