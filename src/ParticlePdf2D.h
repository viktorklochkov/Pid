#ifndef PID_SRC_PARTICLEPDF2D_H_
#define PID_SRC_PARTICLEPDF2D_H_

#include <TAxis.h>

#include <utility>
#include "ParticlePdf1D.h"
namespace Pid{

class ParticlePdf2DBase{
 public:
  ParticlePdf2DBase() = default;
  virtual ~ParticlePdf2DBase() = default;

  virtual float GetPdfValue(float x, float y) const = 0;
  virtual const ParticlePdf1DBase* GetPdf1D(float x) const = 0;

 protected:
  float min_x_{0.f};
  float max_x_{0.f};
  bool is_init_{false};
};

class ParticlePdf2DBinned : public ParticlePdf2DBase {
 public:
  ParticlePdf2DBinned() = default;
  ParticlePdf2DBinned(std::vector<ParticlePdf1DBase*> pdfs, const TAxis& x_axis) :
    pdfs_(std::move(pdfs)),
    x_axis_(x_axis) {}

  ~ParticlePdf2DBinned() override = default;

  float GetPdfValue(float x, float y) const override {
    const auto* pdf = GetPdf1D(x);
    return pdf != nullptr ? pdf->GetPdfValue(y) : 0.f;
  }

  const ParticlePdf1DBase* GetPdf1D(float x) const override {
    const int bin = FindBin(x);
    return bin>=0 && bin<pdfs_.size() ? pdfs_.at(FindBin(x)) : nullptr;
  }

  int FindBin(float x) const {
    return x_axis_.FindBin(x);
  }

 protected:
  std::vector<ParticlePdf1DBase*> pdfs_{};
  TAxis x_axis_{};
};
}
#endif //PID_SRC_PARTICLEPDF2D_H_
