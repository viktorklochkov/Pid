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

    void range(const Fct_t& min, const Fct_t &max) {
        parType_ = EType::kRange;
        fMin_ = min;
        fMax_ = max;
    }

    template <typename T, typename V>
    void range(T min, V max) {
        range(wrap(min), wrap(max));
    }

    void fix(const Fct_t &fix) {
        parType_ = EType::kFix;
        fFix_ = fix;
    }

    template <typename T>
    void fix(T _fix) {
        fix(wrap(_fix));
    }

    void unmanaged() {
        parType_ = EType::kNone;
    }

    void apply(double x) {
        if (parType_ == EType::kFix) {
            var_->setVal(fFix_(x));
            var_->setConstant(kTRUE);
        } else if (parType_ == EType::kRange) {
            assert(fMin_(x) <= fMax_(x));
            var_->setConstant(kFALSE);
            var_->setRange(fMin_(x), fMax_(x));
        } else {
            var_->setConstant(kFALSE);
        }
    }

private:
    static Fct_t wrap(TGraph *gr) {
        assert(gr);
        return [=] (double x) { return gr->Eval(x); };
    }

    static Fct_t wrap(double v) {
        return [=] (double /* x */) { return v; };
    }

    static Fct_t wrap(TF1 *tf) {
        assert(tf);
        return [=] (double x) { return tf->Eval(x); };
    }

    RooRealVar *var_{nullptr};
    EType parType_{EType::kNone};

    Fct_t fMin_;
    Fct_t fMax_;
    Fct_t fFix_;

};





#endif //PID_PARAMETRIZATION_H
