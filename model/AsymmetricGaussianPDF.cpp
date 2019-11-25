//
// Created by eugene on 25/11/2019.
//

#include "DeDxShineFitModel.h"
#include "AsymmetricGaussianPDF.h"

ClassImp(AsymmetricGaussianPDF)

Double_t AsymmetricGaussianPDF::evaluate() const {
  return AsymmetricGaussianFCT(x_, mu_, sigma_, d_);
}

AsymmetricGaussianPDF::AsymmetricGaussianPDF(const char *name,
                                             const char *title,
                                             RooAbsReal &_x,
                                             RooAbsReal &_mu,
                                             RooAbsReal &_sigma,
                                             RooAbsReal &_d) : RooAbsPdf(name, title),
                                                         x_("x", "Observable", this, _x),
                                                         mu_("mu_", "Mean", this, _mu),
                                                         sigma_("sigma", "Width", this, _sigma),
                                                         d_("d", "Asymmetry factor", this, _d)
                                                               {


}
TObject *AsymmetricGaussianPDF::clone(const char *newname) const {
  return new AsymmetricGaussianPDF(*this, newname);
}

double AsymmetricGaussianFCT(double x, double mean, double sigma, double d) {
  double delta = x > mean ? 1 + d : 1 - d;

  double factor = 1. / (sigma * TMath::Sqrt(TMath::TwoPi()));
  double expo = TMath::Exp(-0.5 * ((x - mean) * (x - mean) / delta / delta / sigma / sigma));
  return factor * expo;
}