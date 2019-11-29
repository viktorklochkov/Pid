//
// Created by eugene on 28/11/2019.
//

#include "FitParameterResult.h"

void FitParameterResult::pickFitResultAt(double x) {
    assert(var_);

    xV_.push_back(x);
    parValV_.push_back(var_->getVal());
    parErrV_.push_back(var_->getError());
}

void FitParameterResult::clearFitResult() {
    xV_.clear();
    parValV_.clear();
    parErrV_.clear();
}

TGraph *FitParameterResult::toTGraph() const {
    return new TGraphErrors(xV_.size(), xV_.data(), parValV_.data(), nullptr, parErrV_.data());
}
