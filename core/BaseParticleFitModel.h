//
// Created by eugene on 26/11/2019.
//

#ifndef PID_MODEL_PARTICLEFITMODEL_H_
#define PID_MODEL_PARTICLEFITMODEL_H_

#include <RooAbsPdf.h>

#include <bb/BetheBlochHelper.h>
#include <TDatabasePDG.h>
#include <sstream>

class BaseParticleFitModel {

public:
    BaseParticleFitModel(int pdgID) : pdgID_(pdgID) {}

    void fillParticleInfoFromDB(TDatabasePDG *pdgDB = TDatabasePDG::Instance());

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

    void setParPrefix(const std::string &parPrefix) {
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

    virtual bool isDefinedAt(double x) { return xmin_ <= x && x <= xmax_; }
    virtual void initModel() {};
    virtual RooAbsPdf *getFitModel() = 0;
    virtual std::vector<RooAbsReal *> getFitParams() = 0;

private:
    int pdgID_;
    double mass_{0.};
    double charge_{0.};
    std::string name_{""};
    std::string parPrefix_{""};

    double xmin_{0.};
    double xmax_{0.};

    RooAbsReal *observable_{nullptr};
};

#endif //PID_MODEL_PARTICLEFITMODEL_H_
