//
// Created by eugene on 25/11/2019.
//

#include "AsymmetricGaussianPDF.h"

ClassImp(AsymmetricGaussianPDF)

Double_t AsymmetricGaussianPDF::evaluate() const {
  return AsymmetricGaussianFCT(x_, mu_, sigma_, d_);
}

AsymmetricGaussianPDF::AsymmetricGaussianPDF(const char *name,
                                             const char *title,
                                             RooAbsReal &x,
                                             RooAbsReal &mu,
                                             RooAbsReal &sigma,
                                             RooAbsReal &d) : RooAbsPdf(name, title),
                                                              x_("x", "Observable", this, x),
                                                              mu_("mu_", "Mean", this, mu),
                                                              sigma_("sigma", "Width", this, sigma),
                                                              d_("d", "Asymmetry factor", this, d)
                                                               {}


TObject *AsymmetricGaussianPDF::clone(const char *newname) const {
  return new AsymmetricGaussianPDF(*this, newname);
}

double AsymmetricGaussianFCT(double x, double mean, double sigma, double d) {
  double delta = x > mean ? 1 + d : 1 - d;
  double arg = x - mean;
  double d2 = delta*delta;
  double sigma2 = sigma*sigma;

  double factor = 1. / (sigma * TMath::Sqrt(TMath::TwoPi()));
  double expo = TMath::Exp(-0.5 * (arg * arg / d2 / sigma2));
  return factor * expo;
}