//
// Created by eugene on 27/11/2019.
//

#include "Parametrization.h"

Parametrization::Fct_t Parametrization::wrap(TGraph *gr) {
    assert(gr);
    return [=] (double x) { return gr->Eval(x); };
}

Parametrization::Fct_t Parametrization::wrap(double v) {
    return [=] (double /* x */) { return v; };
}

Parametrization::Fct_t Parametrization::wrap(TF1 *tf) {
    assert(tf);
    return [=] (double x) { return tf->Eval(x); };
}

void Parametrization::apply(double x) {
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

void Parametrization::range(const Parametrization::Fct_t &min, const Parametrization::Fct_t &max) {
    parType_ = EType::kRange;
    fMin_ = min;
    fMax_ = max;
}

void Parametrization::fix(const Parametrization::Fct_t &fix) {
    parType_ = EType::kFix;
    fFix_ = fix;
}
