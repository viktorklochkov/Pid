//
// Created by eugene on 26/11/2019.
//

#ifndef PID_MODEL_PARTICLEFITMODEL_H_
#define PID_MODEL_PARTICLEFITMODEL_H_

#include <RooAbsPdf.h>

#include <bb/BetheBlochHelper.h>
#include <TDatabasePDG.h>

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

    double xmin_{0.};
    double xmax_{0.};

    RooAbsReal *observable_{nullptr};
};

#endif //PID_MODEL_PARTICLEFITMODEL_H_
