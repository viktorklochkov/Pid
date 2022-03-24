//
// Created by eugene on 28/11/2019.
//

#ifndef PID_FITPARAMETER_H
#define PID_FITPARAMETER_H

#include <RooAbsPdf.h>
#include <RooRealVar.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TVirtualPad.h>
#include <algorithm>
#include <functional>
#include <utility>

/**
 *
 */
class FitParameter {

 public:
  enum class EConstraintType {
    kNone,
    kRange,
    kFix,
    kObservable
  };

  typedef std::function<double(double)> ConstraintFct_t;

  struct RangedConstraint_t {
    EConstraintType type_{EConstraintType::kNone};
    ConstraintFct_t lo_;
    ConstraintFct_t hi_;
    ConstraintFct_t fix_;

    double min_ = -std::numeric_limits<double>::infinity();
    double max_ = std::numeric_limits<double>::infinity();

    bool contains(double x) {
      return min_ <= x && x < max_;
    }

    static RangedConstraint_t NoneConstraint(double min, double max) noexcept {
      RangedConstraint_t c{};
      c.min_ = min;
      c.max_ = max;
      return c;
    }

    static RangedConstraint_t NoneConstraint() noexcept {
      RangedConstraint_t c{};
      return c;
    }

    static RangedConstraint_t NONE_CONSTRAINT;
  };

  FitParameter(RooRealVar* var, std::string name) : name_(std::move(name)), var_(var) {}

  explicit FitParameter(const std::string& name) : name_(name),
                                                   var_(new RooRealVar(name.c_str(), "", -RooNumber::infinity(), RooNumber::infinity())) {}

  const std::string& getName() const {
    return name_;
  }

  void setName(const std::string& name) {
    name_ = name;
  }

  RooRealVar* getVar() const {
    return var_;
  }

  void fixWithFitResults();

  void applyConstraint(double x);

  template<typename T, typename V>
  void range(T lo, V hi,
             double min = -RooNumber::infinity(),
             double max = RooNumber::infinity()) {
    range(wrap(lo), wrap(hi), min, max);
  }

  template<typename T>
  void fix(T _fix,
           double min = -RooNumber::infinity(),
           double max = RooNumber::infinity()) {
    fix(wrap(_fix), min, max);
  }

  template<typename T>
  void fixTol(T _fix, double tol,
              double min = -RooNumber::infinity(),
              double max = RooNumber::infinity()) {
    fixTol(wrap(_fix), tol, min, max);
  }

  void range(const ConstraintFct_t&& lo, const ConstraintFct_t&& hi,
             double min = -RooNumber::infinity(),
             double max = RooNumber::infinity());

  void fix(const ConstraintFct_t&& fix,
           double min = -RooNumber::infinity(),
           double max = RooNumber::infinity());

  void fixTol(const ConstraintFct_t&& fix, double relTol,
              double min = -RooNumber::infinity(),
              double max = RooNumber::infinity());

  void reset_constraints() {
    constraints_.clear();
  }

  void pickFitResultAt(double x);

  void clearFitResult();

  TGraph* toTGraph() const;

  void dumpResult(TDirectory& d, TVirtualPad* pad = nullptr);

  RangedConstraint_t& findConstraint(double x);

 private:
  bool checkConstraintRange(double min, double max) {
    if (min >= max) {
      return false;
    }

    for (auto& c : constraints_) {
      bool isOk = c.min_ >= max || c.max_ <= min;
      if (!isOk) return false;
    }

    return true;
  }

  /* base objects */
  std::string name_{""};
  RooRealVar* var_;

  /* constraints */
  static ConstraintFct_t wrap(TGraph* gr);

  static ConstraintFct_t wrap(double v);

  static ConstraintFct_t wrap(TF1* tf);

  static ConstraintFct_t wrap(const std::string& formulaStr);

  std::vector<RangedConstraint_t> constraints_;

  /* fit results */
  std::vector<float> xV_;
  std::vector<float> parValV_;
  std::vector<float> parErrV_;

  TGraphErrors grPARvsX_;
  TGraphErrors grCONSTRvsX_;
};

class FitParameterRegistry {

 public:
  inline std::vector<FitParameter>::iterator par_begin() { return parameters_.begin(); }
  inline std::vector<FitParameter>::iterator par_end() { return parameters_.end(); }
  inline std::vector<FitParameter>::const_iterator par_cbegin() const { return parameters_.cbegin(); }
  inline std::vector<FitParameter>::const_iterator par_cend() const { return parameters_.cend(); }

  FitParameter* par_find(const std::string& name) {
    auto it = std::find_if(par_begin(), par_end(), [name](const FitParameter& p) { return name == p.getName(); });
    return it == par_end() ? nullptr : it.operator->();
  }

  FitParameter* par_find(void* ptr) {
    auto it = std::find_if(par_begin(), par_end(), [ptr](const FitParameter& p) { return ptr == p.getVar(); });
    return it == par_end() ? nullptr : it.operator->();
  }

  std::vector<FitParameter*> par_find_pdf(RooAbsPdf& pdf, RooAbsData& data) {
    std::vector<FitParameter*> result;
    auto params_set = pdf.getParameters(data);

    for (size_t ip = 0; ip < params_set->size(); ++ip) {
      auto p_real_var = dynamic_cast<RooRealVar*>(params_set->operator[](ip));

      auto fit_parameter = par_find(p_real_var);
      assert(fit_parameter);
      result.push_back(fit_parameter);
    }

    return result;
  }

  void add_parameter(RooRealVar* v) {
    if (v && !par_find(v)) {
      parameters_.emplace_back(v, v->namePtr()->GetName());
    }
  }

  void add_parameters_from_pdf(RooAbsPdf& pdf, RooAbsData& data) {
    auto params_set = pdf.getParameters(data);

    for (size_t ip = 0; ip < params_set->size(); ++ip) {
      auto p_real_var = dynamic_cast<RooRealVar*>(params_set->operator[](ip));
      add_parameter(p_real_var);
    }
  }

  static FitParameterRegistry& instance();

 private:
  std::vector<FitParameter> parameters_;
};

#endif//PID_FITPARAMETER_H
