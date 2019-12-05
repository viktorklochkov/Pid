//
// Created by eugene on 28/11/2019.
//

#ifndef PID_FITPARAMETER_H
#define PID_FITPARAMETER_H


#include <functional>
#include <utility>
#include <TF1.h>
#include <RooRealVar.h>
#include <TGraphErrors.h>


/**
 *
 */
class FitParameter  {


public:
    FitParameter(RooRealVar *var, std::string name) :
            name_(std::move(name)), var_(var) {}

    explicit FitParameter(const std::string &name) :
            name_(name),
            var_(new RooRealVar(name.c_str(), "", -RooNumber::infinity(), RooNumber::infinity()))
            {}

    const std::string &getName() const {
        return name_;
    }

    RooRealVar *getVar() const {
        return var_;
    }

    void fixWithFitResults();

    enum class EConstraintType {
        kNone,
        kRange,
        kFix,
        kObservable
    };
    typedef std::function<double (double)> ConstraintFct_t;

    void applyConstraint(double x);

    template <typename T, typename V>
    void range(T min, V max) {
        range(wrap(min), wrap(max));
    }

    template <typename T>
    void fix(T _fix) {
        fix(wrap(_fix));
    }

    void unmanaged() {
        parType_ = EConstraintType::kNone;
    }

    void range(const ConstraintFct_t& min, const ConstraintFct_t &max);

    void fix(const ConstraintFct_t &fix);


    void pickFitResultAt(double x);

    void clearFitResult();

    TGraph *toTGraph() const;
private:

    /* base objects */
    std::string name_{""};
    RooRealVar *var_;

    /* constraints */
    static ConstraintFct_t wrap(TGraph *gr);

    static ConstraintFct_t wrap(double v);

    static ConstraintFct_t wrap(TF1 *tf);

    static ConstraintFct_t wrap(const std::string &formulaStr);

    EConstraintType parType_{EConstraintType::kNone};
    ConstraintFct_t fMin_;
    ConstraintFct_t fMax_;
    ConstraintFct_t fFix_;

    /* fit results */
    std::vector<float> xV_;
    std::vector<float> parValV_;
    std::vector<float> parErrV_;

};


#endif //PID_FITPARAMETER_H
