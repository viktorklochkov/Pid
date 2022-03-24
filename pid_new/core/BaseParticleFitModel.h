//
// Created by eugene on 26/11/2019.
//

#ifndef PID_MODEL_PARTICLEFITMODEL_H_
#define PID_MODEL_PARTICLEFITMODEL_H_

#include <Math/Functor.h>

#include <RooAbsPdf.h>
#include <RooExtendPdf.h>
#include <RooRealVar.h>

#include "FitParameter.h"
#include <Math/IFunctionfwd.h>
#include <TDatabasePDG.h>
#include <pid_new/bb/BetheBlochHelper.h>
#include <sstream>

class BaseParticleFitModel {

 public:
  explicit BaseParticleFitModel(int pdgID) : pdgID_(pdgID) {}

  void fillParticleInfoFromDB();

  double getMass() const {
    return mass_;
  }

  void setMass(double mass) {
    mass_ = mass;
  }

  double getCharge() const {
    return charge_;
  }

  void setCharge(double charge) {
    charge_ = charge;
  }

  const std::string& getName() const {
    return name_;
  }

  void setName(const std::string& name) {
    name_ = name;
  }

  const std::string& getParPrefix() const {
    return parPrefix_;
  }

  void setRooVarPrefix(const std::string& parPrefix) {
    parPrefix_ = parPrefix;
  }

  void setRange(double xmin, double xmax) {
    this->xmin_ = xmin;
    this->xmax_ = xmax;
  }

  RooAbsReal* getObservable() const {
    return observable_;
  }

  void setObservable(RooAbsReal* observable) {
    observable_ = observable;
  }

  void initialize();

  void initExtPdf();

  void print();

  FitParameter* addParameter(RooRealVar* var);

  FitParameter* addParameter(const std::string& name);

  FitParameter& parameter(const std::string& name) {
    return parameterMap_.at(name);
  }

  void applyParameterConstraintsAt(double x);

  void pickFitParameterResultsAt(double x);

  void saveModelTo(TDirectory* dir) const;

  virtual bool isDefinedAt(double x) { return xmin_ <= x && x <= xmax_; }
  virtual void initModel(){};
  virtual RooAbsPdf* getFitModel() = 0;

  RooAbsPdf& getExtFitModel() {
    return *extPdf_;
  }

  EColor getModelColor() const {
    const int colors[]{kRed, kGreen + 2, kMagenta, kCyan, kOrange, kAzure};
    return EColor(colors[modelId_ % sizeof(colors)]);
  }

 private:
  size_t modelId_{MODEL_ID_COUNTER++};
  int pdgID_;
  double mass_{0.};
  double charge_{0.};
  std::string name_{""};
  std::string parPrefix_{""};

  double xmin_{0.};
  double xmax_{0.};

  RooAbsReal* observable_{nullptr};

  std::unique_ptr<RooExtendPdf> extPdf_;
  std::map<std::string, FitParameter> parameterMap_;

  static size_t MODEL_ID_COUNTER;
};

#endif//PID_MODEL_PARTICLEFITMODEL_H_
