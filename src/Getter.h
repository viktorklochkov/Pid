/** @file   Getter.h
    @class  Pid::Getter
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to calculate PID probabilities
*/

#ifndef PidGetter_H
#define PidGetter_H 1

#include <vector>
#include <TCutG.h>
#include <TText.h>
#include <TMultiGraph.h>
#include "ParticleFit.h"
#include "TObject.h"

namespace Pid {

/**
 * @brief interface class for Pid getters
 */
class BaseGetter {

 public:

  virtual ~BaseGetter() = default;

  virtual double GetWeight(float var1, float  var2, int pid)  = 0;

  virtual std::map <int, float > GetWeights(float var1, float var2) = 0;

  virtual int GetPid(float var1, float var2, float purity) = 0;

  virtual void Streamer(TBuffer &) {};
};


/**
 * @brief Bayesian Pid getter
 */
class Getter : public TObject, public BaseGetter {
 public:

  void AddParticle(const ParticleFit &particle, uint id) { species_[id] = particle; }
  void AddParticles(std::map<int, ParticleFit> &&species) { species_ = species; }

  std::map<int, float> GetBayesianProbability(float p, float m2);
  void SetRange(float min, float max) { minx_ = min, maxx_ = max; }

  std::map<uint, float> GetSigma(float p, float m2) {
    std::map<uint, float> sigma{};

    if (p > maxx_ || p < minx_)
      return sigma;

    for (auto &specie : species_) {
      sigma[specie.first] = abs(m2 - specie.second.GetMean(p)) / specie.second.GetSigma(p);
    }
    return sigma;
  }

  int GetPid(float p, float m2, float purity) override {
    auto prob = GetBayesianProbability(p, m2);
    for (auto &pid_prob : prob)
      if (pid_prob.second >= purity) return pid_prob.first;
    return -1;
  }

  double GetWeight(float var1, float var2, int pid) override {
    // not yet implemented
    return 1.0;
  }
  std::map<int, float> GetWeights(float var1, float var2) override {
    return GetBayesianProbability(var1, var2);
  }

 private:

  std::map<int, ParticleFit> species_{};
  float minx_{-100000.};
  float maxx_{100000.};

 ClassDefOverride(Getter, 1);

};

/**
 * @brief Pid getter based on graphical cut TCutG
 */
class CutGGetter : public TObject, public BaseGetter {

 public:
  void AddParticle(TCutG *cut, int pdgId) {
    if (cut) {
      auto insert_result = species_.insert({pdgId, {cut}});
      if (!insert_result.second) {
        (*insert_result.first).second.push_back(cut);
      }
      return;
    }

    throw std::logic_error("empty TCutG object");
  }

  int GetPid(float var1, float var2, float) override {

    for (const auto &specie : species_) {
      int pdgId = specie.first;
      auto specieCuts = specie.second;

      for (auto cut : specieCuts) {
        if (cut->IsInside(var1, var2)) {
          return pdgId;
        }
      }
    }

    return -1;
  }

  double GetWeight(float var1, float var2, int pid) override {
    return 1.0*(GetPid(var1, var2, 1) == pid);
  }

  std::map<int, float> GetWeights(float var1, float var2) override {
    std::map<int, float> result;

    for (const auto &specie : species_) {
      result.insert({specie.first, GetWeight(var1, var2, specie.first)});
    }

    return std::map<int, float>();
  }

  void Draw(Option_t *option = "") override {
    TObject::Draw(option);

    TMultiGraph mg("mg","");
    TText pdgLabel;

    for (const auto &specie : species_) {
      auto specieCuts = specie.second;
      for (auto cut : specieCuts) mg.Add(cut);
    }

    mg.DrawClone(option);

    for (const auto &specie : species_) {
      int pdgId = specie.first;
      auto specieCuts = specie.second;

      double xc, yc;
      for (auto cut : specieCuts) {
        cut->Center(xc, yc);
        pdgLabel.DrawText(xc, yc, Form("%d", pdgId));
      }
    }
  }

 protected:
  std::map<int, std::vector<TCutG *>> species_{};

 ClassDefOverride(Pid::CutGGetter, 1)
};

}
#endif // PidGetter_H
