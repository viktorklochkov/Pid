//
// Created by eugene on 25/11/2019.
//

#ifndef PID_MODEL_DEDXSHINEFITMODEL_CPP_ASYMMETRICGAUSSPDF_H_
#define PID_MODEL_DEDXSHINEFITMODEL_CPP_ASYMMETRICGAUSSPDF_H_

#include <TMath.h>
#include <RooAbsPdf.h>
#include <RooRealProxy.h>

/**
 * @brief Asymmetric Gaussian probability density function (raw)
 *
 * @param x
 * @param mean
 * @param sigma
 * @param d
 * @return
 */
double AsymmetricGaussianFCT(double x, double mean, double sigma, double d);

/**
 * @brief Asymmetric Gauss probability density function
 */
class AsymmetricGaussianPDF : public RooAbsPdf {
 public:
  AsymmetricGaussianPDF() = default;
  AsymmetricGaussianPDF(const AsymmetricGaussianPDF &other, const char *name) :
      RooAbsPdf(other, name),
      x_("x", this, other.x_),
      mu_("mu", this, other.mu_),
      sigma_("sigma", this, other.sigma_),
      d_("d", this, other.d_) {}

  AsymmetricGaussianPDF(const char *name,
                        const char *title,
                        RooAbsReal &_x,
                        RooAbsReal &_mu,
                        RooAbsReal &_sigma,
                        RooAbsReal &_d);

  TObject *clone(const char *newname) const override;

 protected:
  Double_t evaluate() const override;

 private:
  RooRealProxy x_;
  RooRealProxy mu_;
  RooRealProxy sigma_;
  RooRealProxy d_;

 ClassDefNV(AsymmetricGaussianPDF, 0)
};

#endif //PID_MODEL_DEDXSHINEFITMODEL_CPP_ASYMMETRICGAUSSPDF_H_
