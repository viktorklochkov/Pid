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
        protons->setParPrefix("p_");
        fitterHelper.addParticleModel(protons);

        protons->print();
    }

    {
        auto pion_pos = new ShineDeDxParticleFitModel(211);
        pion_pos->fillParticleInfoFromDB();
        pion_pos->setRange(0.4, 5.5);
        pion_pos->setParPrefix("pion_pos_");
        fitterHelper.addParticleModel(pion_pos);

        pion_pos->print();
    }

    inputHistogram->RebinX(5);

    auto c = new TCanvas;
    c->Print("output.pdf(", "pdf");



    for (int i = 1; i < inputHistogram->GetXaxis()->GetNbins(); ++i) {
        double x = inputHistogram->GetXaxis()->GetBinCenter(i);

        if (fitterHelper.particlesModelsDefinedAt(x).size() == 0) continue;

        auto py = inputHistogram->ProjectionY("tmp", i, i);
        py->SetDirectory(0);


        auto frame = fitterHelper.getObservable()->frame();
        RooDataHist ds("ds", "", *fitterHelper.getObservable(), py);

        ds.plotOn(frame);
        frame->Draw();
        c->Print("output.pdf", "pdf");
    }

    c->Print("output.pdf)", "pdf");

    return 0;
}
