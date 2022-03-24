//
// Created by eugene on 25/11/2019.
//

#include "AsymmetricGaussianPDF.h"

ClassImp(AsymmetricGaussianPDF)

    Double_t AsymmetricGaussianPDF::evaluate() const {
  return AsymmetricGaussianFCT(x_, mu_, sigma_, d_);
}

AsymmetricGaussianPDF::AsymmetricGaussianPDF(const char* name,
                                             const char* title,
                                             RooAbsReal& x,
                                             RooAbsReal& mu,
                                             RooAbsReal& sigma,
                                             RooAbsReal& d) : RooAbsPdf(name, title),
                                                              x_("x", "Observable", this, x),
                                                              mu_("mu_", "Mean", this, mu),
                                                              sigma_("sigma", "Width", this, sigma),
                                                              d_("d", "Asymmetry factor", this, d) {}

TObject* AsymmetricGaussianPDF::clone(const char* newname) const {
  return new AsymmetricGaussianPDF(*this, newname);
}

Double_t AsymmetricGaussianPDF::analyticalIntegral(Int_t code, const char* rangeName) const {
  const double SQRT_TWO_PI = TMath::Sqrt(TMath::TwoPi());
  const double SQRT_2 = TMath::Sqrt2();

  if (code == 1) {

    double scale = 1.0;
    double xcMin = x_.min(rangeName) - mu_;
    double xcMax = x_.max(rangeName) - mu_;

    /* both in the left part */
    if (xcMax <= 0) {
      double ss = sigma_ * (1 - d_);

      double xiMin = xcMin / ss / SQRT_2;
      double xiMax = xcMax / ss / SQRT_2;

      return scale * 0.5 * (std::erfc(std::abs(xiMin)) - std::erfc(std::abs(xiMax)));
    }
    /* both in the right part */
    else if (xcMin >= 0) {
      double ss = sigma_ * (1 + d_);

      double xiMin = xcMin / ss / SQRT_2;
      double xiMax = xcMax / ss / SQRT_2;

      return scale * 0.5 * (std::erfc(xiMax) - std::erfc(xiMin));
    }
    /* on different sides */
    else {
      assert(xcMin < 0 && xcMax > 0);
      /* integral from xiMin to 0 */
      double xiMin = xcMin / sigma_ / (1 - d_) / SQRT_2;
      double intMin = scale * 0.5 * std::erf(std::abs(xiMin));

      /* integral from 0 to xiMax */
      double xiMax = xcMax / sigma_ / (1 + d_) / SQRT_2;
      double intMax = scale * 0.5 * std::erf(xiMax);

      return intMax + intMin;
    }

    assert(false);
  }
  return RooAbsReal::analyticalIntegral(code, rangeName);
}

double AsymmetricGaussianFCT(double x, double mean, double sigma, double d) {
  double delta = x > mean ? 1 + d : 1 - d;
  double arg = x - mean;
  double d2 = delta * delta;
  double sigma2 = sigma * sigma;

  double factor = 1. / (sigma * TMath::Sqrt(TMath::TwoPi()));
  double expo = TMath::Exp(-0.5 * (arg * arg / d2 / sigma2));
  return factor * expo;
}