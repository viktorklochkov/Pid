//
// Created by eugene on 01/05/2020.
//

#ifndef PID_PARTICLEFITMODEL_H
#define PID_PARTICLEFITMODEL_H

#include "FitParameter.h"

#include <algorithm>
#include <iostream>
#include <regex>

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
    inline RooAbsPdf &pdf() const {
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

    EColor getColor() const {
        return static_cast<EColor>(color_);
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

    std::vector<FitParameter *> parameters() const {
        return FitParameterRegistry::instance().par_find_pdf(*pdf_, *data_);
    }

    std::vector<FitParameter *> parameters_matching(const std::string &regex_string) const {
        std::vector<FitParameter *> result;

        std::regex re(regex_string);

        auto params = parameters();
        std::copy_if(params.begin(), params.end(), std::back_inserter(result), [re](FitParameter *p) -> bool {
            return std::regex_match(p->getName().begin(), p->getName().end(), re);
        });

        return result;
    }



private:

    RooAbsPdf *pdf_{nullptr};
    RooAbsData *data_{nullptr};


    std::string name_{""};
    std::string par_prefix_{""};
    std::string par_suffix_{""};

    double xmin_{0.};
    double xmax_{0.};

    size_t model_id_{N_MODELS++};
    int color_{MODEL_COLORS[model_id_ % MODEL_COLORS.size()]};

    static size_t N_MODELS;
    static std::vector<int> MODEL_COLORS;

};


#endif //PID_PARTICLEFITMODEL_H
