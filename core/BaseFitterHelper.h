//
// Created by eugene on 26/11/2019.
//

#ifndef PID_BASEFITTERHELPER_H
#define PID_BASEFITTERHELPER_H


#include <vector>
#include <RooRealVar.h>
#include <RooAddPdf.h>
#include "BaseParticleFitModel.h"

class BaseFitterHelper {

public:
    struct ParticleFitModelContainer {
        BaseParticleFitModel *model_{nullptr};
        RooRealVar *integral_{nullptr};

        ParticleFitModelContainer(BaseParticleFitModel *model) : model_(model) {
            integral_ = new RooRealVar;
        }
    };

    RooAbsReal *getObservable() const {
        return observable_;
    }

    void setObservable(RooAbsReal *observable) {
        observable_ = observable;
    }

    void initialize() {
        if (!observable_) {
            observable_ = new RooRealVar;
            observable_->SetName("y");
            observable_->SetTitle("Observable");
        }
    }

    std::vector<BaseFitterHelper::ParticleFitModelContainer> particlesModelsDefinedAt(double x) {
        decltype(particleFitModels_) fitModelsAtX;
        std::copy_if(particleFitModels_.begin(), particleFitModels_.end(), std::back_inserter(fitModelsAtX),
                [=](ParticleFitModelContainer &c) {return c.model_->isDefinedAt(x);});
        return fitModelsAtX;
    }

    virtual RooAbsPdf *generateCompositePDF(double x, const char *name = "composite") {
        RooArgList pdfList;
        RooArgList constantList;

        for (auto &model : particlesModelsDefinedAt(x)) {
            /* Unowned objects are inserted with the add() method. Owned objects are added with addOwned() or addClone() */
            pdfList.add(*model.model_->getFitModel());
            constantList.add(*model.integral_);
        }

        return new RooAddPdf(name, "", pdfList, constantList);
    }

    /**
     *  Called after initialize()
     * @param model
     */
    void addParticleModel(BaseParticleFitModel *model) {
        assert(model);

        model->setObservable(observable_);
        model->initialize();

        particleFitModels_.emplace_back(model);
    };

private:
    std::vector<ParticleFitModelContainer> particleFitModels_;

    RooAbsReal *observable_{nullptr};
};


#endif //PID_BASEFITTERHELPER_H
