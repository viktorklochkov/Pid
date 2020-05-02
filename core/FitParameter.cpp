//
// Created by eugene on 28/11/2019.
//

#include "FitParameter.h"

FitParameter::RangedConstraint_t FitParameter::RangedConstraint_t::NONE_CONSTRAINT{FitParameter::RangedConstraint_t::NoneConstraint()};

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
    auto &c = findConstraint(x);

    if (c.type_ == EConstraintType::kFix) {
        var_->setVal(c.fix_(x));
        var_->setConstant(kTRUE);
    } else if (c.type_ == EConstraintType::kRange) {
        assert(c.lo_(x) <= c.hi_(x));
        var_->setConstant(kFALSE);
        var_->setRange(c.lo_(x), c.hi_(x));
        var_->setVal(0.5*(c.lo_(x) + c.hi_(x)));
    } else {
        var_->setConstant(kFALSE);
        var_->removeRange();
    }
}

void FitParameter::range(const ConstraintFct_t &&lo, const ConstraintFct_t &&hi, double min,
                         double max) {
    RangedConstraint_t c;
    c.type_ = EConstraintType::kRange;
    c.min_ = min;
    c.max_ = max;
    c.lo_ = lo;
    c.hi_ = hi;

    if (checkConstraintRange(min, max)) {
        constraints_.push_back(std::move(c));
    } else {
        throw std::runtime_error("Illegal constraint range");
    }
}

void FitParameter::fix(const ConstraintFct_t &&fix, double min, double max) {

    RangedConstraint_t c;
    c.type_ = EConstraintType::kFix;
    c.fix_ = fix;
    c.min_ = min;
    c.max_ = max;

    if (checkConstraintRange(min, max)) {
        constraints_.push_back(std::move(c));
    } else {
        throw std::runtime_error("Illegal constraint range");
    }

}

void FitParameter::pickFitResultAt(double x) {
    xV_.push_back(x);
    parValV_.push_back(var_->getVal());
    parErrV_.push_back(var_->getError());

    grPARvsX_.SetPoint(grPARvsX_.GetN(), x, var_->getVal());
    grPARvsX_.SetPointError(grPARvsX_.GetN() - 1, 0., var_->getError());

    if (findConstraint(x).type_ == EConstraintType::kRange) {
        double constraint_lo = findConstraint(x).lo_(x);
        double constraint_hi = findConstraint(x).hi_(x);
        grCONSTRvsX_.SetPoint(grCONSTRvsX_.GetN(), x, 0.5 * (constraint_lo + constraint_hi));
        grCONSTRvsX_.SetPointError(grCONSTRvsX_.GetN() - 1, 0, 0.5 * (constraint_hi - constraint_lo));
    } else if (findConstraint(x).type_ == EConstraintType::kFix) {
        double fix_val = findConstraint(x).fix_(x);
        grCONSTRvsX_.SetPoint(grCONSTRvsX_.GetN(), x, fix_val);
    }
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

FitParameter::RangedConstraint_t &FitParameter::findConstraint(double x) {
    RangedConstraint_t *result = nullptr;
    for (auto &c : constraints_) {
        if (c.contains(x)) {
            if (result) {
                throw std::runtime_error("More then one constraint contains x = " + std::to_string(x));
            }

            result = &c;
        }
    }

    if (!result) {
        return RangedConstraint_t::NONE_CONSTRAINT;
    }

    return *result;
}

void FitParameter::fixTol(const FitParameter::ConstraintFct_t &&fix, double relTol, double min, double max) {
    RangedConstraint_t c;
    c.type_ = EConstraintType::kRange;
    c.lo_ = [=] (double x) -> double { return (1 - relTol)*fix(x); };
    c.hi_ = [=] (double x) -> double { return (1 + relTol)*fix(x); };
    c.min_ = min;
    c.max_ = max;

    if (checkConstraintRange(min,max)) {
        constraints_.push_back(std::move(c));
    } else {
        throw std::runtime_error("Illegal constraint range");
    }

}


FitParameterRegistry &FitParameterRegistry::instance() {
    static FitParameterRegistry registry;
    return registry;
}
