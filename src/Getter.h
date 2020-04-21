/** @file   Getter.h
    @class  Pid::Getter
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to calculate PID probabilities
*/

#ifndef PidGetter_H
#define PidGetter_H 1

#include <vector>

#include "TObject.h"

#include "ParticlePdf2D.h"

namespace Pid {

/**
 * @brief interface class for Pid getters
 */
class BaseGetter {
 public:
  virtual ~BaseGetter() = default;
  virtual double GetWeight(double var1, double var2, int pid) = 0;
  virtual std::map<int, double> GetWeights(double var1, double var2) = 0;
  virtual int GetPid(double var1, double var2, double purity) = 0;
  virtual void Streamer(TBuffer &) {};
};

/**
 * @brief Bayesian Pid getter
 */
class Getter : public TObject, public BaseGetter {
 public:

  void AddParticle(ParticlePdf2DBase* particle, uint id) { species_[id] = particle; }
  void AddParticles(std::map<int, ParticlePdf2DBase*> &&species) { species_ = species; }

  std::map<int, double> GetBayesianProbability(double p, double m2) ;
  void SetRange(double min, double max) { minx_ = min, maxx_ = max; }

  int GetPid(double p, double m2, double purity) override {
    auto prob = GetBayesianProbability(p, m2);
    for (auto &pid_prob : prob)
      if (pid_prob.second >= purity) return pid_prob.first;
    return -1;
  }

  double GetWeight(double var1, double var2, int pid) override {
    // not yet implemented
    return 1.0;
  }

  std::map<int, double> GetWeights(double var1, double var2)  override {
    return GetBayesianProbability(var1, var2);
  }

 private:

  std::map<int, ParticlePdf2DBase*> species_{};
  double minx_{-100000.};
  double maxx_{100000.};

 ClassDefOverride(Getter, 1);

};


}
#endif // PidGetter_H
