#ifndef PID_SRC_PARTICLEPDF1D_H_
#define PID_SRC_PARTICLEPDF1D_H_

#include "TF1.h"

namespace Pid {

class ParticlePdf1DBase {
 public:
  ParticlePdf1DBase() = default;
  virtual ~ParticlePdf1DBase() = default;

  virtual float GetPdfValue(float y) = 0;

 protected:
  bool is_init_{false};
};

class ParticlePdf1D : public ParticlePdf1DBase {
 public:
  ParticlePdf1D() = default;
  ParticlePdf1D(const TF1& pdf) : pdf_(pdf), is_init_(true) {}

  float GetPdfValue(float y) final {
    return pdf_.Eval(y);
  }

 protected:
  TF1 pdf_;
  bool is_init_{false};
};
}
#endif //PID_SRC_PARTICLEPDF1D_H_
