//
// Created by eugene on 26/11/2019.
//

#ifndef PID_FITTERHELPER_H
#define PID_FITTERHELPER_H


#include <vector>
#include <RooRealVar.h>
#include <RooAddPdf.h>
#include <RooExtendPdf.h>
#include <RooFormulaVar.h>
#include "BaseParticleFitModel.h"

struct FitContext_t {
    double x{0.};
    bool isEmpty{true};

    std::unique_ptr<RooAbsPdf> pdf;
    std::vector<BaseParticleFitModel *> fitModels;
    std::vector<std::unique_ptr<RooAbsReal>> purityVars;

    void applyConstraints() {
        for (auto m : fitModels) {
            m->applyParameterConstraintsAt(this->x);
        }
    }

    void pickFitResults() {
        for (auto m : fitModels) {
            m->pickFitParameterResultsAt(this->x);
        }
    }
};


class FitterHelper {

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
            observable_ = new RooRealVar("y", "dE/dx", 0.0, "MIP");
        }
    }

    std::vector<FitterHelper::ParticleFitModelContainer> particlesModelsDefinedAt(double x) const {
        decltype(particleFitModels_) fitModelsAtX;
        std::copy_if(particleFitModels_.begin(), particleFitModels_.end(), std::back_inserter(fitModelsAtX),
                     [=](const ParticleFitModelContainer &c) { return c.model_->isDefinedAt(x); });
        return fitModelsAtX;
    }

    RooAbsPdf *compositePDF(double x, const char *name = "composite") const {
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

    void saveAllTo(TDirectory *dir) const {
        assert(dir);

        for (auto &m : particleFitModels_) {
            m.model_->saveModelTo(dir);
        }

    }

    FitContext_t getFitContext(double x) const {
        FitContext_t context;
        context.x = x;

        for (auto &model : particleFitModels_) {
            if (model.model_->isDefinedAt(x)) {
                context.fitModels.push_back(model.model_);
            }
        }

        if (!context.fitModels.empty()) {
            context.isEmpty = false;
            context.pdf.reset(compositePDF(x));

            for (auto m : context.fitModels) {
                context.purityVars.emplace_back(
                        new RooFormulaVar(Form("purity_%s", m->getName().c_str()), "", "@0/@1",
                                          RooArgList(m->getExtFitModel(), *(context.pdf))));
            }
        }

        return context;
    }


private:
    std::vector<ParticleFitModelContainer> particleFitModels_;

    RooRealVar *observable_{nullptr};
};


#endif //PID_FITTERHELPER_H
