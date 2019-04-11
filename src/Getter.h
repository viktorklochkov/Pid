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

class BaseGetter {

 public:
  virtual int GetPid(float var1, float var2, float purity) = 0;

  virtual void Streamer(TBuffer &) {};
};

class Getter : public TObject, public BaseGetter {
 public:

  void AddParticle(const ParticleFit &particle, uint id) { species_[id] = particle; }
  void AddParticles(std::map<uint, ParticleFit> &&species) { species_ = species; }

  std::map<uint, float> GetBayesianProbability(float p, float m2);
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
    if (prob[PidParticles::kPion] > purity) return 211;
    if (prob[PidParticles::kProton] > purity) return 2212;
    if (prob[PidParticles::kKaon] > purity) return 321;
    return -1;
  }

 private:

  std::map<uint, ParticleFit> species_{};
  float minx_{-100000.};
  float maxx_{100000.};

 ClassDef(Getter, 1);

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

  void Draw(Option_t *option = "") override {
    TObject::Draw(option);

    TMultiGraph mg("mg","");
    TText pdgLabel;

    for (const auto &specie : species_) {
      auto specieCuts = specie.second;
      for (auto cut : specieCuts) mg.Add(cut);
    }

    mg.DrawClone("AL");

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

 ClassDef(Pid::CutGGetter, 1)
};

}
#endif // PidGetter_H
