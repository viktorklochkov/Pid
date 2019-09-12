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

namespace Pid {

class Getter : public TObject {
 public:

  /**   Default constructor   **/
  Getter();

  void AddParticle(const ParticleFit &particle, uint id) { species_[id] = particle; }
  void AddParticles(std::map<uint, ParticleFit> &&species) { species_ = species; }

  std::map<uint, double> GetBayesianProbability(double p, double m2);
  void SetRange(double min, double max) { minx_ = min, maxx_ = max; }

  std::map<uint, double> GetSigma(double p, double m2) {
    std::map<uint, double> sigma{};

    if (p > maxx_ || p < minx_)
      return sigma;

    for (auto &specie : species_) {
      sigma[specie.first] = abs(m2 - specie.second.GetMean(p)) / specie.second.GetSigma(p);
    }
    return sigma;
  }

  int GetPid(double p, double m2, double purity) {
    auto prob = GetBayesianProbability(p, m2);
    if (prob[PidParticles::kPion] > purity) return 211;
    if (prob[PidParticles::kProton] > purity) return 2212;
    if (prob[PidParticles::kKaon] > purity) return 321;
    return -1;
  }

 private:

  std::map<uint, ParticleFit> species_{};
  double minx_{-100000.};
  double maxx_{100000.};

 ClassDef(Getter, 1);

};

}
#endif // PidGetter_H
