//
// Created by eugene on 26/11/2019.
//

#ifndef PID_BASEFITTERHELPER_H
#define PID_BASEFITTERHELPER_H


#include <vector>
#include <RooRealVar.h>
#include <RooAddPdf.h>
#include <RooExtendPdf.h>
#include "BaseParticleFitModel.h"

class BaseFitterHelper {

public:
    struct ParticleFitModelContainer {
        BaseParticleFitModel *model_{nullptr};

        explicit ParticleFitModelContainer(BaseParticleFitModel *model) : model_(model) {}
    };

    RooRealVar *getObservable() const {
        return observable_;
    }

    void initialize() {
        if (!observable_) {
            observable_ = new RooRealVar("y", "Observable", 0.0, "MIP");
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

        auto modelsAtX = particlesModelsDefinedAt(x);

        for (auto &model : modelsAtX) {
            /* Unowned objects are inserted with the add() method. Owned objects are added with addOwned() or addClone() */
            pdfList.add(model.model_->getExtFitModel());
        }

        return new RooAddPdf(name, "", pdfList);
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

    void at(double x) {
        x_ = x;
        compositePdfAtX_.reset(generateCompositePDF(x));
        modelsAtX_ = particlesModelsDefinedAt(x);
    }

    void applyAllParameterConstraints(double x) {
        for (auto m : particlesModelsDefinedAt(x)) {
            m.model_->applyParameterConstraintsAt(x);
        }
    }

    void applyAllParameterConstraints() {
        applyAllParameterConstraints(x_);
    }

    void pickAllFitResults() {
        for (auto &m: modelsAtX_) {
            m.model_->pickFitParameterResultsAt(x_);
        }
    }

    void saveAllTo(TDirectory *dir) const {
        assert(dir);

        for (auto &m : particleFitModels_) {
            m.model_->saveModelTo(dir);
        }

    }

    double getX() const {
        return x_;
    }

    const std::shared_ptr<RooAbsPdf> &getCompositePdfAtX() const {
        return compositePdfAtX_;
    }

    const std::vector<ParticleFitModelContainer> &getModelsAtX() const {
        return modelsAtX_;
    }


private:
    std::vector<ParticleFitModelContainer> particleFitModels_;

    RooRealVar *observable_{nullptr};

    double x_{0.};
    std::shared_ptr<RooAbsPdf> compositePdfAtX_;
    std::vector<ParticleFitModelContainer> modelsAtX_;
};


#endif //PID_BASEFITTERHELPER_H
