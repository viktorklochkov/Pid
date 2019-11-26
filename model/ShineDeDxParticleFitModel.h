//
// Created by eugene on 26/11/2019.
//

#ifndef PID_SHINEDEDXPARTICLEFITMODEL_H
#define PID_SHINEDEDXPARTICLEFITMODEL_H


#include <core/BaseParticleFitModel.h>

class ShineDeDxParticleFitModel : public BaseParticleFitModel {

public:
    ShineDeDxParticleFitModel(int pdgId);

    void initModel() override;

    RooAbsPdf *getFitModel() override;

    std::vector<RooAbsReal *> getFitParams() override;

private:
    RooAbsPdf *pdf_;
    RooRealVar *bb;
    RooRealVar *sigma;
    RooRealVar *d;

};


#endif //PID_SHINEDEDXPARTICLEFITMODEL_H
