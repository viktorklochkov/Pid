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
    inline FitParameter& parameter(const std::string& pname) {
        auto find_it = std::find_if(parameters_.begin(), parameters_.end(), [=] (const FitParameter* p) { return p->getName() == par_prefix_ + pname; });
        if (find_it == parameters_.end()) {
            throw std::runtime_error("Parameter '" + pname + "' is not found in model '" + name_ + "'");
        }

        return *(*find_it);
    }


private:

    RooAbsPdf *pdf_{nullptr};


    std::string name_{""};
    std::string par_prefix_{""};
    std::string par_suffix_{""};

    double xmin_{0.};
    double xmax_{0.};

    std::vector<FitParameter*> parameters_;

};


#endif //PID_PARTICLEFITMODEL_H
