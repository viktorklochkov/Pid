/** @file   ParticleFit.h
    @class  Pid::ParticleFit
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to store fit resuls for particle specie
*/

#ifndef PidParticleFit_H
#define PidParticleFit_H 1

#include <vector>
#include <array>

#include "TF1.h"
#include "Constants.h"

namespace Pid {

class ParticleFit {
 public:

  /**   Default constructor   **/
  ParticleFit();
  explicit ParticleFit(int type) : particle_type_(type) {};

  std::vector<double> GetFunctionParams(double p) const;
  double Eval(double p, double m2) ;

  void SetParametrization(const std::vector<TF1> &parametrization) { parametrization_ = parametrization; }
  void SetFitFunction(const TF1 &function) { function_ = function; }
  void SetRange(double min, double max) { minx_ = min, maxx_ = max; }
  void SetIsFitted(bool is = true) { isfitted_ = is; }
  void SetIsFixed(const std::vector<bool> &is) { isfixed_ = is; }

  const TF1 &GetFunction() const { return function_; }
  int GetNpar() const { return function_.GetNpar(); }
  TF1 &GetParametrizationFunction(int ipar) { return parametrization_.at(ipar); }
  bool GetIsFixed(uint ipar) const {
    if (ipar >= isfixed_.size())
      return false;
    return isfixed_.at(ipar);
  }

  double GetSigma(double p) const { return parametrization_.at(PidFunction::kSigma).Eval(p); }
  double GetMean(double p) const { return parametrization_.at(PidFunction::kMean).Eval(p); }

 private:

  TF1 function_;
  std::vector<TF1> parametrization_{};
  std::vector<bool> isfixed_{};

  double minx_{-1.};
  double maxx_{-1.};

  int npoints_{-1};
  int particle_type_{-1};

  bool isfitted_{false};

 	ClassDef(ParticleFit,2);

};

}
#endif // PidParticleFit_H
