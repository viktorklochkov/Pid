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
    ParticleFitModel(std::string name, RooAbsPdf &pdf, RooAbsData &data,
                     std::string par_prefix = "",
                     std::string par_suffix = "");

    /* fitting ranges */
    void setRange(double xmin, double xmax) {
        xmin_ = xmin;
        xmax_ = xmax;
    }

    inline bool isDefinedAt(double x) const { return xmin_ <= x && x <= xmax_; }

    /* accessors */
    inline RooAbsPdf &getPdf() const {
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
    inline FitParameter &parameter(const std::string &pname) {

        for (auto it = FitParameterRegistry::instance().par_begin();
            it != FitParameterRegistry::instance().par_end(); ++it) {
            if (it->getName() == par_prefix_ + pname) {
                return it.operator*();
            }
        }

        throw std::runtime_error("Parameter '" + par_prefix_ + pname + "' is not found in the registry");
    }


private:

    RooAbsPdf *pdf_{nullptr};


    std::string name_{""};
    std::string par_prefix_{""};
    std::string par_suffix_{""};

    double xmin_{0.};
    double xmax_{0.};

};


#endif //PID_PARTICLEFITMODEL_H
