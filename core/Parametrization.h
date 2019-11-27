//
// Created by eugene on 27/11/2019.
//

#ifndef PID_PARAMETRIZATION_H
#define PID_PARAMETRIZATION_H

#include <functional>
#include <TF1.h>
#include <RooRealVar.h>


class Parametrization {

public:
    enum class EType {
        kNone,
        kRange,
        kFix,
        kObservable
    };

    typedef std::function<double (double)> Fct_t;

    explicit Parametrization(RooRealVar *var) : var_(var) {}

    RooRealVar *getVar() const {
        return var_;
    }

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
        parType_ = EType::kNone;
    }

    void apply(double x);

private:
    static Fct_t wrap(TGraph *gr);

    static Fct_t wrap(double v);

    static Fct_t wrap(TF1 *tf);

    RooRealVar *var_{nullptr};
    EType parType_{EType::kNone};

    Fct_t fMin_;
    Fct_t fMax_;
    Fct_t fFix_;

};





#endif //PID_PARAMETRIZATION_H
