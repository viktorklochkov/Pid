//
// Created by eugene on 28/11/2019.
//

#ifndef PID_FITPARAMETERRESULT_H
#define PID_FITPARAMETERRESULT_H


#include <RooRealVar.h>
#include <TGraphErrors.h>

class FitParameterResult {

public:
    explicit FitParameterResult(RooRealVar *var) : var_(var) {}

    void pickFitResultAt(double x);

    void clearFitResult();

    TGraph *toTGraph() const;


private:
    RooRealVar *var_;

    std::vector<float> xV_;
    std::vector<float> parValV_;
    std::vector<float> parErrV_;
};


#endif //PID_FITPARAMETERRESULT_H
