//
// Created by eugene on 28/11/2019.
//

#include "FitParameter.h"

void FitParameter::fixWithFitResults() {
    fix(toTGraph());
}

FitParameter::ConstraintFct_t FitParameter::wrap(TGraph *gr) {
    assert(gr);
    return [=](double x) { return gr->Eval(x); };
}

FitParameter::ConstraintFct_t FitParameter::wrap(double v) {
    return [=](double /* x */) { return v; };
}

FitParameter::ConstraintFct_t FitParameter::wrap(TF1 *tf) {
    assert(tf);
    return [=](double x) { return tf->Eval(x); };
}

void FitParameter::applyConstraint(double x) {
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

void FitParameter::range(const FitParameter::ConstraintFct_t &min, const FitParameter::ConstraintFct_t &max) {
    parType_ = EConstraintType::kRange;
    fMin_ = min;
    fMax_ = max;
}

void FitParameter::fix(const FitParameter::ConstraintFct_t &fix) {
    parType_ = EConstraintType::kFix;
    fFix_ = fix;
}

void FitParameter::pickFitResultAt(double x) {
    assert(var_);

    xV_.push_back(x);
    parValV_.push_back(var_->getVal());
    parErrV_.push_back(var_->getError());
}

void FitParameter::clearFitResult() {
    xV_.clear();
    parValV_.clear();
    parErrV_.clear();
}

TGraph *FitParameter::toTGraph() const {
    return new TGraphErrors(xV_.size(), xV_.data(), parValV_.data(), nullptr, parErrV_.data());
}

FitParameter::ConstraintFct_t FitParameter::wrap(const std::string &formula) {
    /* TODO fix up this memory leakage */
    auto fct = new TF1("tmp", formula.c_str());
    return wrap(fct);
}
