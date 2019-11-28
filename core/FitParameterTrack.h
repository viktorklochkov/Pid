//
// Created by eugene on 28/11/2019.
//

#ifndef PID_FITPARAMETERTRACK_H
#define PID_FITPARAMETERTRACK_H


#include <RooRealVar.h>
#include <TGraphErrors.h>

class FitParameterTrack {

public:
    explicit FitParameterTrack(RooRealVar *var) : var_(var) {}

    void addTrackPoint(double x) {
        assert(var_);

        xV_.push_back(x);
        parValV_.push_back(var_->getVal());
        parErrV_.push_back(var_->getError());
    }

    void clear() {
        xV_.clear();
        parValV_.clear();
        parErrV_.clear();
    }

    TGraph *toTGraph() {
        return new TGraphErrors(xV_.size(), xV_.data(), parValV_.data(), nullptr, parErrV_.data());
    }


private:
    RooRealVar *var_;

    std::vector<float> xV_;
    std::vector<float> parValV_;
    std::vector<float> parErrV_;
};


#endif //PID_FITPARAMETERTRACK_H
