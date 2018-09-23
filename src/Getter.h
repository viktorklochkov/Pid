/** @file   Getter.h
    @class  Pid::Getter
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
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
    
    std::map<uint, float> GetBayesianProbability(float p, float m2);
    void SetRange(float min, float max) { minx_ = min, maxx_ = max; }
    
    std::map<uint, float> GetSigma(float p, float m2)
    {
        std::map<uint, float> sigma{};

        if (p>maxx_ || p<minx_) 
            return sigma;
        
        for (auto &specie : species_)
        {
            sigma[specie.first] = abs(m2 - specie.second.GetMean(p)) / specie.second.GetSigma(p);
        }
        return sigma;
    }
    
    int GetPid(float p, float m2, float purity)
    {
        auto prob = GetBayesianProbability(p, m2);
        if (prob[PidParticles::kPion] > purity) return 211;
        if (prob[PidParticles::kProton] > purity) return 2212;
        if (prob[PidParticles::kKaon] > purity) return 311;
        return -1;
    }
    
private:
    
    std::map<uint, ParticleFit> species_{};
    float minx_{-100000.};
    float maxx_{100000.};
    
ClassDef(Getter,1);

};

}
#endif // PidGetter_H
