//
// Created by eugene on 28/11/2019.
//

#ifndef PID_FITPARAMETER_H
#define PID_FITPARAMETER_H


#include "FitParameterConstraint.h"
#include "FitParameterTrack.h"

class FitParameter : public FitParameterConstraint, public FitParameterTrack {


public:
    FitParameter(RooRealVar *var, const std::string &name) :
            FitParameterConstraint(var),
            FitParameterTrack(var),
            name_(name), var_(var) {}

    const std::string &getName() const {
        return name_;
    }

    RooRealVar *getVar() const {
        return var_;
    }

private:
    std::string name_{""};
    RooRealVar *var_;
};


#endif //PID_FITPARAMETER_H
