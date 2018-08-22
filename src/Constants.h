/** @file   Constants.h
    @author Viktor Klochkov (klochkov44@gmail.com)
    @date   August 2018
    @brief  Some constants and enumerators
*/


#ifndef PidConstants_H
#define PidConstants_H 1

const double SpeedOfLight = 29.9792458;

namespace PidParticles{
    enum eParticle{
        kProton = 0,
        kPion,
        kKaon,
        kBg,
        nParticles
    };
    
    const float masses[nParticles] = { 0.938, 0.140, 0.5, 0.0 };
    
}

#endif

