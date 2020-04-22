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
class FitParameter {


public:
    enum class EConstraintType {
        kNone,
        kRange,
        kFix,
        kObservable
    };

    typedef std::function<double(double)> ConstraintFct_t;

    struct RangedConstraint_t {
        EConstraintType type_{EConstraintType::kNone};
        ConstraintFct_t lo_;
        ConstraintFct_t hi_;
        ConstraintFct_t fix_;

        double min_ = -std::numeric_limits<double>::infinity();
        double max_ = std::numeric_limits<double>::infinity();

        bool contains(double x) {
            return min_ <= x && x < max_;
        }

        static RangedConstraint_t NoneConstraint(double min, double max) noexcept {
            RangedConstraint_t c{};
            c.min_ = min;
            c.max_ = max;
            return c;
        }

        static RangedConstraint_t NoneConstraint() noexcept {
            RangedConstraint_t c{};
            return c;
        }

        static RangedConstraint_t NONE_CONSTRAINT;
    };

    FitParameter(RooRealVar *var, std::string name) :
            name_(std::move(name)), var_(var) {}

    explicit FitParameter(const std::string &name) :
            name_(name),
            var_(new RooRealVar(name.c_str(), "", -RooNumber::infinity(), RooNumber::infinity())) {}

    const std::string &getName() const {
        return name_;
    }

    RooRealVar *getVar() const {
        return var_;
    }

    void fixWithFitResults();


    void applyConstraint(double x);

    template<typename T, typename V>
    void range(T lo, V hi,
               double min = -RooNumber::infinity(),
               double max = RooNumber::infinity()
    ) {
        range(wrap(lo), wrap(hi), min, max);
    }

    template<typename T>
    void fix(T _fix,
             double min = -RooNumber::infinity(),
             double max = RooNumber::infinity()
    ) {
        fix(wrap(_fix), min, max);
    }

    template<typename T>
    void fixTol(T _fix, double tol,
                double min = -RooNumber::infinity(),
                double max = RooNumber::infinity()
    ) {
        fixTol(wrap(_fix), tol, min, max);
    }

    void range(const ConstraintFct_t &&lo, const ConstraintFct_t &&hi,
               double min = -RooNumber::infinity(),
               double max = RooNumber::infinity()
    );

    void fix(const ConstraintFct_t &&fix,
             double min = -RooNumber::infinity(),
             double max = RooNumber::infinity()
    );

    void fixTol(const ConstraintFct_t &&fix, double relTol,
                double min = -RooNumber::infinity(),
                double max = RooNumber::infinity()
    );


    void pickFitResultAt(double x);

    void clearFitResult();

    TGraph *toTGraph() const;

private:

    RangedConstraint_t &findConstraint(double x);

    bool checkConstraintRange(double min, double max) {
        if (min >= max) {
            return false;
        }

        for (auto &c : constraints_) {
            bool isOk = c.min_ >= max || c.max_ <= min;
            if (!isOk) return false;
        }

        return true;
    }


    /* base objects */
    std::string name_{""};
    RooRealVar *var_;

    /* constraints */
    static ConstraintFct_t wrap(TGraph *gr);

    static ConstraintFct_t wrap(double v);

    static ConstraintFct_t wrap(TF1 *tf);

    static ConstraintFct_t wrap(const std::string &formulaStr);


    std::vector<RangedConstraint_t> constraints_;

    /* fit results */
    std::vector<float> xV_;
    std::vector<float> parValV_;
    std::vector<float> parErrV_;

};


#endif //PID_FITPARAMETER_H
