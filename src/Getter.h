/** @file   Getter.h
    @author Viktor Klochkov (klochkov44@gmail.com)
    @date   August 2018
    @brief  Class to calculate PID probabilities
*/

#ifndef PidGetter_H
#define PidGetter_H 1

#include <vector>
#include "ParticleFit.h"
#include "TObject.h"

namespace Pid
{

class Getter : public TObject
{
public:
    
	/**   Default constructor   **/
	Getter() ;
    
    void AddParticle (const ParticleFit& particle, uint id) { species_[id] = particle; }
    void AddParticles (std::map<uint, ParticleFit> &&species) { species_ = species; }
    
    std::map<uint, float>&& GetBayesianProbability(float p, float m2);
    
    void SetRange(float min, float max) { minx_ = min, maxx_ = max; }
    
private:
    
    std::map<uint, ParticleFit> species_{};
    
    float minx_{-100000.};
    float maxx_{100000.};
    
ClassDef(Getter,1);

};

}
#endif // PidGetter_H
