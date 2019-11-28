//
// Created by eugene on 27/11/2019.
//

#ifndef PID_FITPARAMETERCONSTRAINT_H
#define PID_FITPARAMETERCONSTRAINT_H

#include <functional>
#include <TF1.h>
#include <RooRealVar.h>


class FitParameterConstraint {

public:
    enum class EConstraintType {
        kNone,
        kRange,
        kFix,
        kObservable
    };

    typedef std::function<double (double)> Fct_t;

    explicit FitParameterConstraint(RooRealVar *var) : var_(var) {}

    void range(const Fct_t& min, const Fct_t &max);

    template <typename T, typename V>
    void range(T min, V max) {
        range(wrap(min), wrap(max));
    }

    void fix(const Fct_t &fix);

    template <typename T>
    void fix(T _fix) {
        fix(wrap(_fix));
    }

    void unmanaged() {
        parType_ = EConstraintType::kNone;
    }

    void applyConstraint(double x);

private:
    static Fct_t wrap(TGraph *gr);

    static Fct_t wrap(double v);

    static Fct_t wrap(TF1 *tf);

    RooRealVar *var_{nullptr};
    EConstraintType parType_{EConstraintType::kNone};

    Fct_t fMin_;
    Fct_t fMax_;
    Fct_t fFix_;

};





#endif //PID_FITPARAMETERCONSTRAINT_H
