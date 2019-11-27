//
// Created by eugene on 26/11/2019.
//

#ifndef PID_MODEL_PARTICLEFITMODEL_H_
#define PID_MODEL_PARTICLEFITMODEL_H_

#include <RooAbsPdf.h>
#include <RooRealVar.h>

#include <bb/BetheBlochHelper.h>
#include <TDatabasePDG.h>
#include <sstream>
#include "Parametrization.h"

class BaseParticleFitModel {

public:
    struct ParameterContainer {
        std::string name_;
        RooRealVar *var_;
        Parametrization *par_;
    };


    BaseParticleFitModel(int pdgID) : pdgID_(pdgID) {}

    void fillParticleInfoFromDB();

    double getMass() const {
        return mass_;
    }

    void setMass(double mass) {
        mass_ = mass;
    }

    double getCharge() const {
        return charge_;
    }

    void setCharge(double charge) {
        charge_ = charge;
    }

    const std::string &getName() const {
        return name_;
    }

    void setName(const std::string &name) {
        name_ = name;
    }

    const std::string &getParPrefix() const {
        return parPrefix_;
    }

    void setRooVarPrefix(const std::string &parPrefix) {
        parPrefix_ = parPrefix;
    }

    void setRange(double xmin, double xmax) {
        this->xmin_ = xmin;
        this->xmax_ = xmax;
    }

    RooAbsReal *getObservable() const {
        return observable_;
    }

    void setObservable(RooAbsReal *observable) {
        observable_ = observable;
    }

    void initialize() {
        assert(name_ != "");

        assert(observable_);
        assert(xmax_ > xmin_);
        assert(mass_ > 0);
        assert(charge_ != 0);

        initModel();

        for (auto par : getFitParams()) {
            std::string newName(getParPrefix() + std::string(par->GetName()));
            par->SetName(newName.c_str());
        }
    }

    void print() {
        std::ostringstream pm;
        pm << name_ << ":" << std::endl;
        pm << "fitting model: ";
        getFitModel()->printClassName(pm);
        pm << std::endl;
        pm << "observable: ";
        getObservable()->printName(pm);
        pm << std::endl;

        std::cout << pm.str();
    }

    RooRealVar *addParameter(RooRealVar *var) {
        assert(var);
        ParameterContainer pc;
        pc.name_ = var->GetName();
        pc.var_ = var;
        pc.par_ = new Parametrization(var);
        parameters_.emplace(pc.name_,pc);

        return var;
    }

    Parametrization *getParByName(const std::string &name) {
        return parameters_.at(name).par_;
    }

    void applyParAt(double x) {
        for (auto &p : parameters_) {
            p.second.par_->apply(x);
        }
    }

    virtual bool isDefinedAt(double x) { return xmin_ <= x && x <= xmax_; }
    virtual void initModel() {};
    virtual RooAbsPdf *getFitModel() = 0;
    virtual std::vector<RooRealVar *> getFitParams() = 0;


private:
    int pdgID_;
    double mass_{0.};
    double charge_{0.};
    std::string name_{""};
    std::string parPrefix_{""};

    double xmin_{0.};
    double xmax_{0.};

    RooAbsReal *observable_{nullptr};

    std::map<std::string, ParameterContainer > parameters_;
};

#endif //PID_MODEL_PARTICLEFITMODEL_H_
