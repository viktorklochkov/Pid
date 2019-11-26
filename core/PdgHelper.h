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
    static double mass(Pdg_t pdg) {
       if (isIon(pdg)) {
           return 0.938*std::get<0>(getAZIon(pdg));
       } else if (getParticlePdg(pdg)) {
           return getParticlePdg(pdg)->Mass();
       }

       throw std::runtime_error("Unknown pdg code");
    }

    static double charge(Pdg_t pdg) {
        if (isIon(pdg)) {
            return 0.938*std::get<1>(getAZIon(pdg));
        } else if (getParticlePdg(pdg)) {
            return getParticlePdg(pdg)->Charge();
        }

        throw std::runtime_error("Unknown pdg code");
    }

    static std::tuple<int, int> getAZIon(Pdg_t pdg) {
        assert(pdg > 1000000000);
        int A = (pdg % 10000)/10;
        int Z = (pdg % 10000000)/10000;
        return {A,Z};
    }

    static int getZ(Pdg_t pdg) {
        return std::get<1>(getAZIon(pdg));
    }

    static int getA(Pdg_t pdg) {
        return std::get<0>(getAZIon(pdg));
    }

    static bool isIon(Pdg_t pdg) {
        return pdg > 1000000000;
    }

    static TParticlePDG* getParticlePdg(Pdg_t pdg) {
        return TDatabasePDG::Instance()->GetParticle(pdg);
    }

};


#endif //PID_PDGHELPER_H
