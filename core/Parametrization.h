//
// Created by eugene on 27/11/2019.
//

#ifndef PID_PARAMETRIZATION_H
#define PID_PARAMETRIZATION_H

#include <functional>
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

    Parametrization(RooRealVar *var) : var_(var) {}

    void range(const Fct_t& min, const Fct_t &max) {
        parType_ = EType::kRange;
        fMin_ = min;
        fMax_ = max;
    }

    void range(TGraph *grMin, TGraph *grMax) {
        range(wrapTGraph(grMin), wrapTGraph(grMax));
    }

    void range(double min, double max) {
        range(wrapDouble(min), wrapDouble(max));
    }

    void fix(const Fct_t &fix) {
        parType_ = EType::kFix;
        fFix_ = fix;
    }

    void unmanaged() {
        parType_ = EType::kNone;
    }

    void apply(double x) {
        if (parType_ == EType::kFix) {
            var_->setVal(fFix_(x));
            var_->setConstant(kTRUE);
        } else if (parType_ == EType::kRange) {
            var_->setRange(fMin_(x), fMax_(x));
        } else {}
    }

private:
    static Fct_t wrapTGraph(TGraph *gr) {
        assert(gr != nullptr);
        return [=] (double x) { return gr->Eval(x); };
    }

    Fct_t wrapDouble(double v) {
        return [=] (double /* x */) { return v; };
    }

    RooRealVar *var_{nullptr};
    EType parType_{EType::kNone};

    Fct_t fMin_;
    Fct_t fMax_;
    Fct_t fFix_;

};





#endif //PID_PARAMETRIZATION_H
