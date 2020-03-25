//
// Created by eugene on 26/11/2019.
//

#include <TFile.h>
#include <TH2D.h>

#include <core/BaseFitterHelper.h>
#include <examples/shine/ShineDeDxParticleFitModel.h>
#include <RooWorkspace.h>
#include <RooDataHist.h>
#include <TCanvas.h>
#include <RooPlot.h>

#include <TMath.h>
#include <TPaveText.h>
#include <core/FitParameter.h>

#include <model/AsymmetricGaussianPDF.h>


double poly(const std::vector<double> &pv, double x) {
    double result = 0.;
    double powX = 1.;

    for (auto p : pv) {
        result += p * powX;
        powX *= x;
    }

    return result;
}

FitParameter::ConstraintFct_t polyF(std::initializer_list<double> params) {
    std::vector<double> pv(params.begin(), params.end());
    return [pv](double x) { return poly(pv, x); };
}

FitParameter::ConstraintFct_t ranged(FitParameter::ConstraintFct_t &&ff, double min, double max, double def) {
    return [=](double x) {
        if (min <= x && x < max) {
            return ff(x);
        }

        return def;
    };

}


FitParameter::ConstraintFct_t wrapToX(FitParameter::ConstraintFct_t f, int charge) {
    return [=](double x) { return f(TMath::Exp(charge * x) / 20); };
}


int main(int argc, char **argv) {

    auto inputFile = TFile::Open(argv[1]);

    TH2D *inputHistogram = nullptr;
    inputFile->GetObject("reco_info/hTrackdEdx_allTPC", inputHistogram);
    inputHistogram->SetDirectory(0);
    inputHistogram->Print();

    BaseFitterHelper fitterHelper;
    fitterHelper.initialize();

    fitterHelper.getObservable()->setRange(0., 4.);


    {
        auto protons = new ShineDeDxParticleFitModel(2212);
        protons->fillParticleInfoFromDB();
        protons->setRange(2.2, 6.4);
        protons->setRooVarPrefix("p_");
        fitterHelper.addParticleModel(protons);

        protons->parameter("integral").range(0, RooNumber::infinity());
        protons->parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(2212), 1));
        protons->parameter("sigma").range(0.05, .11);


        protons->print();
    }

    {
        auto pion_pos = new ShineDeDxParticleFitModel(211);
        pion_pos->fillParticleInfoFromDB();
        pion_pos->setRange(0.4, 5.5);
        pion_pos->setRooVarPrefix("pion_pos_");
        fitterHelper.addParticleModel(pion_pos);

        pion_pos->parameter("integral").fix(
                polyF({7.44815e+03, -3.99161e+04, 7.94035e+04, -7.40455e+04, 3.56406e+04, -5.68673e+03, 4.35015e+01}),
                0.42, 2.68);
        pion_pos->parameter("integral").range(0, RooNumber::infinity(), -RooNumber::infinity(), 0.42);
        pion_pos->parameter("integral").range(0, RooNumber::infinity(), 2.68, RooNumber::infinity());
        pion_pos->parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(211), 1));
//        pion_pos->parameter("sigma").range(0.05, .13);
        pion_pos->parameter("sigma").fix("1.51831e-01 - 2.69106e-02*x + 2.70373e-03*x*x");


        pion_pos->print();
    }

    {
        auto pion_neg = new ShineDeDxParticleFitModel(-211);
        pion_neg->fillParticleInfoFromDB();
        pion_neg->setRange(-5.5, -0.4);
        pion_neg->setRooVarPrefix("pion_neg_");
        fitterHelper.addParticleModel(pion_neg);

        pion_neg->parameter("integral").fix(polyF({
                                                          2.98904e+04,
                                                          1.79762e+05,
                                                          4.11753e+05,
                                                          4.64306e+05,
                                                          2.85236e+05,
                                                          9.17047e+04,
                                                          1.52933e+04,
                                                          1.23289e+03,
                                                          3.66055e+01
                                                  }));
        pion_neg->parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(-211), -1));
//        pion_neg->parameter("sigma").range(0.02, 0.13);
        pion_neg->parameter("sigma").fix("1.51831e-01 + 2.69106e-02*x + 2.70373e-03*x*x");
        pion_neg->parameter("d").range(0, 0.1);

        pion_neg->print();
    }

//    {
//        auto kaon_neg = new ShineDeDxParticleFitModel(-321);
//        kaon_neg->fillParticleInfoFromDB();
//        kaon_neg->setRange(-5.5, -3.);
//        kaon_neg->setRooVarPrefix("kaon_neg_");
//        fitterHelper.addParticleModel(kaon_neg);
//
//        kaon_neg->parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(-321), -1));
//        kaon_neg->parameter("sigma").range(0.02, 0.13);
//
//        kaon_neg->print();
//    }

    {
        auto deuteron = new ShineDeDxParticleFitModel(1000010020);
        deuteron->fillParticleInfoFromDB();
        deuteron->setRange(3, 3.8);
        deuteron->setRooVarPrefix("deuteron_");
        fitterHelper.addParticleModel(deuteron);

        deuteron->parameter("integral").range(0, RooNumber::infinity());
        deuteron->parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(1000010020), 1));
        deuteron->parameter("sigma").range(0.05, .3);

        deuteron->print();
    }


    {
        auto positron = new ShineDeDxParticleFitModel(-11);
        positron->fillParticleInfoFromDB();
        positron->setRange(0.4, 4.);
        positron->setRooVarPrefix("positron_");
        fitterHelper.addParticleModel(positron);

        positron->parameter("integral").range(0, RooNumber::infinity());
        positron->parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(11), 1));
        positron->parameter("sigma").range(0.07, 0.13);

        positron->print();
    }

    {
        auto electron = new ShineDeDxParticleFitModel(11);
        electron->fillParticleInfoFromDB();
        electron->setRange(-4.51, 0.);
        electron->setRooVarPrefix("electron_");
        fitterHelper.addParticleModel(electron);

        electron->parameter("integral").range(0, RooNumber::infinity());
        electron->parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(11), -1));
        electron->parameter("sigma").range(0.03, 0.1);

        electron->print();
    }

    inputHistogram->RebinX(3);
    inputHistogram->RebinY(2);

    auto c = new TCanvas;
    c->Print("output.pdf(", "pdf");

    inputHistogram->Draw("colz");


    for (int i = 1; i < inputHistogram->GetXaxis()->GetNbins(); ++i) {
        double x = inputHistogram->GetXaxis()->GetBinCenter(i);

        if (fitterHelper.particlesModelsDefinedAt(x).empty()) continue;

        auto py = inputHistogram->ProjectionY("tmp", i, i);
        py->SetDirectory(0);
        if (py->Integral() < 1000) continue;

        fitterHelper.at(x);
        fitterHelper.applyAllParameterConstraints();

        auto frame = fitterHelper.getObservable()->frame();

        RooDataHist ds("ds", "", *fitterHelper.getObservable(), py);
        ds.plotOn(frame);

        auto model = fitterHelper.getCompositePdfAtX();
        model->fitTo(ds, RooFit::Extended());


        fitterHelper.pickAllFitResults();

        model->plotOn(frame);
        for (auto &m : fitterHelper.particlesModelsDefinedAt(x)) {
            m.model_->getExtFitModel().plotOn(frame, RooFit::LineColor(kRed),
                                              RooFit::Normalization(1.0, RooAbsReal::RelativeExpected));
        }
        frame->Draw();

        auto text = new TText(0.7, 0.8, Form("x = %f", x));
        text->SetNDC();
        text->Draw();

        gPad->SetLogy();

        c->Print("output.pdf", "pdf");

    }

    c->Print("output.pdf)", "pdf");

    TFile outputFile("output.root", "RECREATE");
    fitterHelper.saveAllTo(&outputFile);

    return 0;
}
