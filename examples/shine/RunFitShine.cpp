//
// Created by eugene on 26/11/2019.
//

#include <TFile.h>
#include <TH2D.h>

#include <core/BaseFitterHelper.h>
#include <model/ShineDeDxParticleFitModel.h>
#include <RooWorkspace.h>
#include <RooDataHist.h>
#include <TCanvas.h>
#include <RooPlot.h>

#include <TMath.h>
#include <TPaveText.h>

Parametrization::Fct_t wrapToX(Parametrization::Fct_t f, int charge) {
    return [=] (double x) { return f(TMath::Exp(charge*x)/20); };
}

int main(int argc, char ** argv) {

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
        protons->setRange(1.5, 6.4);
        protons->setRooVarPrefix("p_");
        fitterHelper.addParticleModel(protons);

        protons->getParByName("bb")->fix(wrapToX(BetheBlochHelper::makeBBForPdg(2212),1));


        protons->print();
    }

    {
        auto pion_pos = new ShineDeDxParticleFitModel(211);
        pion_pos->fillParticleInfoFromDB();
        pion_pos->setRange(0.4, 5.5);
        pion_pos->setRooVarPrefix("pion_pos_");
        fitterHelper.addParticleModel(pion_pos);

        pion_pos->getParByName("bb")->fix(wrapToX(BetheBlochHelper::makeBBForPdg(211), 1));

        pion_pos->print();
    }

    {
        auto pion_pos = new ShineDeDxParticleFitModel(-11);
        pion_pos->fillParticleInfoFromDB();
        pion_pos->setRange(0.8, 4.);
        pion_pos->setRooVarPrefix("positron_");
        fitterHelper.addParticleModel(pion_pos);

        pion_pos->getParByName("bb")->fix(wrapToX(BetheBlochHelper::makeBBForPdg(11), 1));

        pion_pos->print();
    }

    inputHistogram->RebinX(3);
    inputHistogram->RebinY(2);

    auto c = new TCanvas;
    c->Print("output.pdf(", "pdf");



    for (int i = 1; i < inputHistogram->GetXaxis()->GetNbins(); ++i) {
        double x = inputHistogram->GetXaxis()->GetBinCenter(i);

        if (fitterHelper.particlesModelsDefinedAt(x).size() == 0) continue;

        fitterHelper.applyAllParametrizations(x);

        auto py = inputHistogram->ProjectionY("tmp", i, i);
        py->SetDirectory(0);

        if (py->Integral() < 1000) continue;


        auto frame = fitterHelper.getObservable()->frame();

        RooDataHist ds("ds", "", *fitterHelper.getObservable(), py);
        ds.plotOn(frame);


        for (auto m : fitterHelper.particlesModelsDefinedAt(x)) {
            m.model_->getFitModel()->chi2FitTo(ds);
        }

        auto model = fitterHelper.generateCompositePDF(x);
        model->chi2FitTo(ds);

        model->plotOn(frame);
        frame->Draw();

        auto text  = new TText(0.7, 0.8, Form("x = %f", x));
        text->SetNDC();
        text->Draw();



        c->Print("output.pdf", "pdf");


        delete model;
    }

    c->Print("output.pdf)", "pdf");

    return 0;
}
