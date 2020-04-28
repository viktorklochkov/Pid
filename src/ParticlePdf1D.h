#ifndef PID_SRC_PARTICLEPDF1D_H_
#define PID_SRC_PARTICLEPDF1D_H_

#include "TF1.h"
#include "Constants.h"

namespace Pid {

class ParticlePdf1DBase {
 public:
  ParticlePdf1DBase() = default;
  virtual ~ParticlePdf1DBase() = default;

  virtual float GetPdfValue(float y) const = 0;

 protected:
  bool is_init_{false};
};

class ParticlePdf1D : public ParticlePdf1DBase {
 public:
  ParticlePdf1D() = default;
  ParticlePdf1D(const TF1& pdf, const std::vector<double>& par, PdgCode_t pdg) : pdf_(pdf), pdg_(pdg), is_init_(true) {
    assert(pdf_.GetNpar() == par.size());
    pdf_.SetParameters(&par[0]);
  }

  float GetPdfValue(float y) const final {
    return pdf_.Eval(y);
  }

  PdgCode_t GetPdg() const {
    return pdg_;
  }

 protected:
  TF1 pdf_;
  PdgCode_t pdg_{-1};
  bool is_init_{false};
};
}
#endif //PID_SRC_PARTICLEPDF1D_H_
