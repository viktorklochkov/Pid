//
// Created by eugene on 01/05/2020.
//

#ifndef PID_PARTICLEFITMODEL_H
#define PID_PARTICLEFITMODEL_H

#include "FitParameter.h"

#include <algorithm>
#include <iostream>
#include <RooAbsPdf.h>

class ParticleFitModel {

public:
    ParticleFitModel(const std::string &name, RooAbsPdf &pdf, RooAbsData &data, const std::string &par_prefix = "",
                     const std::string &par_suffix = "");

    /* fitting ranges */
    void setRange(double xmin, double xmax) {
        xmin_ = xmin;
        xmax_ = xmax;
    }

    inline bool isDefinedAt(double x) const { return xmin_ <= x && x <= xmax_; }

    /* accessors */
    inline RooAbsPdf& getPdf() const {
        return *pdf_;
    }

    const std::string &getName() const {
        return name_;
    }

    const std::string &getParPrefix() const {
        return par_prefix_;
    }

    const std::string &getParSuffix() const {
        return par_suffix_;
    }

    /* parameter accessors */
    inline std::vector<FitParameter>::iterator par_begin() { return parameters_.begin(); }
    inline std::vector<FitParameter>::iterator par_end() { return parameters_.end(); }
    inline std::vector<FitParameter>::const_iterator par_cbegin() { return parameters_.cbegin(); }
    inline std::vector<FitParameter>::const_iterator par_cend() { return parameters_.cend(); }
    inline FitParameter& parameter(const std::string& name) {
        auto find_it = std::find_if(par_begin(), par_end(), [name] (const FitParameter& p) { return p.getName() == name; });
        if (find_it == par_end()) {
            throw std::runtime_error("Parameter '" + name + "' is not found");
        }

        return *find_it;
    }

    /* constraints */
    inline void applyConstraints(double x) {
        std::for_each(par_begin(), par_end(), [x] (FitParameter &p) { p.applyConstraint(x); });
    }

    /* pick par values */
    inline void pickParValues(double x) {
        std::for_each(par_begin(), par_end(), [x] (FitParameter &p) { p.pickFitResultAt(x); });
    }

private:
    void stripParNames();

    RooAbsPdf *pdf_{nullptr};

    std::vector<FitParameter> parameters_;

    std::string name_{""};
    std::string par_prefix_{""};
    std::string par_suffix_{""};

    double xmin_{0.};
    double xmax_{0.};

};


#endif //PID_PARTICLEFITMODEL_H
