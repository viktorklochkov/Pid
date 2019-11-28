//
// Created by eugene on 27/11/2019.
//

#include "FitParameterConstraint.h"

FitParameterConstraint::Fct_t FitParameterConstraint::wrap(TGraph *gr) {
    assert(gr);
    return [=] (double x) { return gr->Eval(x); };
}

FitParameterConstraint::Fct_t FitParameterConstraint::wrap(double v) {
    return [=] (double /* x */) { return v; };
}

FitParameterConstraint::Fct_t FitParameterConstraint::wrap(TF1 *tf) {
    assert(tf);
    return [=] (double x) { return tf->Eval(x); };
}

void FitParameterConstraint::applyConstraint(double x) {
    if (parType_ == EConstraintType::kFix) {
        var_->setVal(fFix_(x));
        var_->setConstant(kTRUE);
    } else if (parType_ == EConstraintType::kRange) {
        assert(fMin_(x) <= fMax_(x));
        var_->setConstant(kFALSE);
        var_->setRange(fMin_(x), fMax_(x));
    } else {
        var_->setConstant(kFALSE);
    }
}

void FitParameterConstraint::range(const FitParameterConstraint::Fct_t &min, const FitParameterConstraint::Fct_t &max) {
    parType_ = EConstraintType::kRange;
    fMin_ = min;
    fMax_ = max;
}

void FitParameterConstraint::fix(const FitParameterConstraint::Fct_t &fix) {
    parType_ = EConstraintType::kFix;
    fFix_ = fix;
}
