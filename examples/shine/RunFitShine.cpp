//
// Created by eugene on 26/11/2019.
//

#include <TFile.h>
#include <TH2D.h>

#include <core/FitterHelper.h>
#include <examples/shine/ShineDeDxParticleFitModel.h>
#include <RooWorkspace.h>
#include <RooDataHist.h>
#include <TCanvas.h>
#include <RooPlot.h>
#include <TLegend.h>

#include <TMath.h>
#include <TPaveText.h>
#include <core/FitParameter.h>

#include <model/AsymmetricGaussianPDF.h>
#include <core/ParticleFitModel.h>

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

    /* Building RooFit model */

    /* observable */
    RooRealVar v_dEdx("x", "dEdx", 0, 4.0, "MIP");
    RooDataHist dh_dEdx("dh_dEdx", "", v_dEdx);

    /* common for all particle species asymmetry parameter */
    RooRealVar v_d("d", "#delta", -0.5, 0.5, "");

    /* negative pions model */
    RooRealVar v_pion_neg_bb("pion_neg_bb", "BB(#pi^{-})", 0, "MIP");
    RooRealVar v_pion_neg_sigma("pion_neg_sigma", "#sigma(#pi^{-})", 0, "MIP");
    RooRealVar v_pion_neg_n("pion_neg_n", "N(#pi^{-})", 0, "");
    AsymmetricGaussianPDF pdf_pion_neg("pion_neg", "", v_dEdx, v_pion_neg_bb, v_pion_neg_sigma, v_d);
    RooExtendPdf epdf_pion_neg("ext_pion_neg", "", pdf_pion_neg, v_pion_neg_n);

    /* negative kaons model */
    RooRealVar v_kaon_neg_bb("kaon_neg_bb", "BB(K^{-})", 0, "MIP");
    RooFormulaVar v_kaon_neg_sigma("kaon_neg_sigma", "#sigma(K^{-})", "@0*TMath::Power(@1/@2, 0.65)",
                                   RooArgSet(v_pion_neg_sigma, v_kaon_neg_bb, v_pion_neg_bb));
    RooRealVar v_kaon_neg_n("kaon_neg_n", "N(K^{-})", 0, "");
    AsymmetricGaussianPDF pdf_kaon_neg("kaon_neg", "", v_dEdx, v_kaon_neg_bb, v_kaon_neg_sigma, v_d);
    RooExtendPdf epdf_kaon_pdf("ext_kaon_neg", "", pdf_kaon_neg, v_kaon_neg_n);

    /* electrons model */
    RooRealVar v_electron_bb("electron_bb", "BB(e^{-})", 0, "MIP");
    RooFormulaVar v_electron_sigma("electron_sigma", "#sigma(e^{-})", "@0*TMath::Power(@1/@2, 0.65)",
                                   RooArgSet(v_pion_neg_sigma, v_electron_bb, v_pion_neg_bb));
    RooRealVar v_electron_n("electron_n", "N(e^{-})", 0, "");
    AsymmetricGaussianPDF pdf_electron("electron", "", v_dEdx, v_electron_bb, v_electron_sigma, v_d);
    RooExtendPdf epdf_electron("ext_electron", "", pdf_electron, v_electron_n);

    std::list<ParticleFitModel> fit_models;

    fit_models.emplace_back("pion_neg", epdf_pion_neg, dh_dEdx, "pion_neg_");
    fit_models.back().setRange(-5.5, -0.4);
    fit_models.back().parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(-211), -1));
    fit_models.back().parameter("sigma").fixTol(polyF({3.34221e-01, 2.10099e-01, 6.72043e-02, 9.48219e-03, 4.98327e-04}), 0.05, -5.5, -3.5);

    fit_models.emplace_back("kaon_neg", epdf_kaon_pdf, dh_dEdx, "kaon_neg_");
    fit_models.back().setRange(-5.0, -2.5);
    fit_models.back().parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(-321), -1));

    fit_models.emplace_back("electron", epdf_electron, dh_dEdx, "electron_");
    fit_models.back().setRange(-5., -0.4);
    fit_models.back().parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(-11), -1));

    FitParameterRegistry::instance().par_find("d")->range(0., 0.3);


    /* IO */
    auto inputFile = TFile::Open(argv[1]);

    TH2D *inputHistogram = nullptr;
    inputFile->GetObject("reco_info/hTrackdEdx_allTPC", inputHistogram);
    inputHistogram->SetDirectory(nullptr);
    inputHistogram->Print();


    FitterHelper fitterHelper;
    fitterHelper.initialize();

    fitterHelper.getObservable()->setRange(0., 4.);

    /*
    {
        auto protons = new ShineDeDxParticleFitModel(2212);
        protons->fillParticleInfoFromDB();
        protons->setRange(2.33, 6.4);
        protons->setRooVarPrefix("p_");
        fitterHelper.addParticleModel(protons);

        protons->parameter("integral").range(0, RooNumber::infinity());
        protons->parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(2212), 1));
        protons->parameter("sigma").fixTol(polyF({-7.55547e+00, 1.66733e+01, -8.22026e+00, -8.80845e-01, 1.71284e+00, -4.63780e-01, 4.04260e-02}), 0.05, 2.25, 3.3);
        protons->parameter("sigma").fixTol(polyF({2.37175e-01, -3.64606e-02}), 0.05, 3.3, 4.0);
        protons->parameter("sigma").fixTol(polyF({1.07984e+00, -5.29675e-01, 9.23235e-02, -5.41696e-03}), 0.05, 4., RooNumber::infinity());


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
        pion_neg->parameter("sigma").fixTol("1.51831e-01 + 2.69106e-02*x + 2.70373e-03*x*x", 0.05);
        pion_neg->parameter("d").range(0, 0.1);

        pion_neg->print();
    }

    {
        auto kaon_neg = new ShineDeDxParticleFitModel(-321);
        kaon_neg->fillParticleInfoFromDB();
        kaon_neg->setRange(-5.5, -3.);
        kaon_neg->setRooVarPrefix("kaon_neg_");
        fitterHelper.addParticleModel(kaon_neg);

        kaon_neg->parameter("integral").range(0, RooNumber::infinity());
        kaon_neg->parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(-321), -1));
        kaon_neg->parameter("sigma").range(0.02, 0.13);

        kaon_neg->print();
    }

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
        electron->parameter("sigma").range(0.03, 0.09, -RooNumber::infinity(), -1.4);
        electron->parameter("sigma").fixTol(polyF({1.66744e-01, 4.52550e-02}), 0.01,-1.4,RooNumber::infinity());

        electron->print();
    }
    */

    inputHistogram->RebinX(1);
    inputHistogram->RebinY(1);

    auto c = new TCanvas;
    c->Print("output.pdf(", "pdf");

    inputHistogram->Draw("colz");
    gPad->SetLogz();
    inputHistogram->GetYaxis()->SetRangeUser(0, 4.);

    {
        TF1 bbPionNeg("bbPionNeg", [](const Double_t *x, const Double_t *p) {
            return wrapToX(BetheBlochHelper::makeBBForPdg(-211), -1).operator()(x[0]);
        }, -6., -0.5, 0);
        bbPionNeg.SetLineColor(kRed);
        bbPionNeg.Draw("same");

        TF1 bbPionPos("bbPionPos", [](const Double_t *x, const Double_t *p) {
            return wrapToX(BetheBlochHelper::makeBBForPdg(211), 1).operator()(x[0]);
        }, 0.5, 6, 0);
        bbPionPos.SetLineColor(kMagenta + 1);
        bbPionPos.Draw("same");

        TF1 bbProton("bbProton", [](const Double_t *x, const Double_t *p) {
            return wrapToX(BetheBlochHelper::makeBBForPdg(2212), 1).operator()(x[0]);
        }, 2.5, 6, 0);
        bbProton.SetLineColor(kBlack);
        bbProton.Draw("same");

        TF1 bbPositron("bbPositron", [](const Double_t *x, const Double_t *p) {
            return wrapToX(BetheBlochHelper::makeBBForPdg(11), 1).operator()(x[0]);
        }, 0., 5., 0);
        bbPositron.SetLineColor(kBlue + 1);
        bbPositron.Draw("same");


        TF1 bbElectron("bbElectron", [](const Double_t *x, const Double_t *p) {
            return wrapToX(BetheBlochHelper::makeBBForPdg(-11), -1).operator()(x[0]);
        }, -5., 0., 0);
        bbElectron.SetLineColor(kBlue + 1);
        bbElectron.Draw("same");

        TF1 bbDeuteron("bbDeuteron", [](const Double_t *x, const Double_t *p) {
            return wrapToX(BetheBlochHelper::makeBBForPdg(1000010020), 1).operator()(x[0]);
        }, 3., 6., 0);
        bbDeuteron.SetLineColor(kOrange + 1);
        bbDeuteron.Draw("same");

        TF1 bbKaonPos("bbKaonPos", [](const Double_t *x, const Double_t *p) {
            return wrapToX(BetheBlochHelper::makeBBForPdg(321), 1).operator()(x[0]);
        }, 2., 5., 0);
        bbKaonPos.SetLineColor(kCyan + 1);
        bbKaonPos.Draw("same");

        TF1 bbKaonNeg("bbKaonNeg", [](const Double_t *x, const Double_t *p) {
            return wrapToX(BetheBlochHelper::makeBBForPdg(-321), -1).operator()(x[0]);
        }, -5., -2., 0);
        bbKaonNeg.SetLineColor(kCyan + 1);
        bbKaonNeg.Draw("same");

        c->Print("output.pdf(", "pdf");
        c->Clear();
    }


    for (int i = 1; i < inputHistogram->GetXaxis()->GetNbins(); ++i) {
        double x = inputHistogram->GetXaxis()->GetBinCenter(i);
        std::cout << "Bin: " << i << "; x = " << x << std::endl;

//        if (fitterHelper.particlesModelsDefinedAt(x).empty()) continue;

        /* building composite model */
        RooArgList composite_model_components;
        std::for_each(fit_models.begin(), fit_models.end(), [x, &composite_model_components](ParticleFitModel &m) {
            if (m.isDefinedAt(x)) {
                composite_model_components.add(m.getPdf());
            }
        });

        if (composite_model_components.empty()) {
            std::cout << "Nothing to fit in this bin" << std::endl;
            continue;
        }

        RooAddPdf pdf_composite(Form("composite_%d", i), "", composite_model_components);
        ParticleFitModel m_composite("composite", pdf_composite, dh_dEdx);

        double slice_integral = 0.;
        /* preparing input */
        {
            auto py = inputHistogram->ProjectionY("tmp", i, i);
            py->SetDirectory(nullptr);

            dh_dEdx.reset();
            RooDataHist slice_data("slice_dh", "", v_dEdx, py);
            dh_dEdx.add(slice_data);

            slice_integral = py->Integral();

            delete py;
        }

        /* set range for the component integrals */
        auto integral_params = m_composite.parameters_matching(".*_n$");
        std::for_each(integral_params.begin(), integral_params.end(), [=] (FitParameter* p) { p->reset_constraints(); p->range(0., slice_integral); });
        /* constraints */
        auto model_params = m_composite.parameters();
        std::for_each(model_params.begin(), model_params.end(), [=](FitParameter *p) { p->applyConstraint(x); });

        m_composite.getPdf().fitTo(dh_dEdx, RooFit::Extended());
        std::for_each(model_params.begin(), model_params.end(), [=] (FitParameter *p) { p->pickFitResultAt(x); });

        auto frame_data_fit = v_dEdx.frame();
        dh_dEdx.plotOn(frame_data_fit);
        m_composite.getPdf().plotOn(frame_data_fit, RooFit::Name(m_composite.getName().c_str()));


        c->Divide(2,2);

        c->cd(1);
        frame_data_fit->Draw();

        c->cd(2);
        frame_data_fit->Draw();
        gPad->SetLogy(1);






        c->Print("output.pdf", "pdf");
        c->Clear();


        /*
        RooDataHist ds("ds", "", *fitterHelper.getObservable(), py);
        ds.plotOn(frame);

        context.pdf->fitTo(ds, RooFit::Extended());
        context.pickFitResults();


        context.pdf->plotOn(frame, RooFit::Name("fit_composite"));

        for (auto &m : context.fitModels) {
            m->getExtFitModel().plotOn(frame,
                                       RooFit::LineStyle(kDashed),
                                       RooFit::LineColor(m->getModelColor()),
                                       RooFit::Normalization(1., RooAbsReal::RelativeExpected),
                                       RooFit::Name(m->getName().c_str())
            );
        }


        c->Divide(2, 2);
        c->cd(1);
        gPad->SetLogy();

        frame->Draw();

        c->cd(2);
        frame->Draw();

        auto legend = new TLegend(0.1, 0.8, 0.9, 1.);
        legend->SetNColumns(2);
        legend->SetHeader(Form("x = %f", x));
        legend->AddEntry(frame->findObject("fit_composite"), "Composite fit", "l");

        for (auto &m : context.fitModels) {
            legend->AddEntry(frame->findObject(m->getName().c_str()), m->getName().c_str(), "l");
        }

        legend->Draw();

        c->cd(3);

        auto framePurity = frame->emptyClone("purity");
        framePurity->SetTitle("Purity");
        RooRealVar compositeIntegral("compositeIntegral", "",
                                     context.pdf->expectedEvents(RooArgSet(*fitterHelper.getObservable())));

        for (auto &m : context.fitModels) {
            auto componentIntegral = m->parameter("integral").getVar();
            RooFormulaVar purityVar(Form("purity_%s", m->getName().c_str()), "purity", "@0*@1/(@2*@3)*((@0*@1) > 1)",
                                    RooArgSet(
                                            m->getExtFitModel(),
                                            *componentIntegral,
                                            *context.pdf,
                                            compositeIntegral
                                    ));

            purityVar.plotOn(framePurity, RooFit::LineColor(m->getModelColor()));
        }
        framePurity->Draw();


        c->cd(4);

        inputHistogram->Draw("colz");
        inputHistogram->GetYaxis()->SetRangeUser(0, 4);
        TLine l;
        l.SetLineColor(kRed);
        l.SetLineWidth(2);
        l.DrawLine(x, 0., x, 4.);
        gPad->SetLogz();

        c->Print("output.pdf", "pdf");
        c->Clear();

        delete py;
*/
    }


    TFile outputFile("output.root", "RECREATE");
    std::for_each(FitParameterRegistry::instance().par_begin(), FitParameterRegistry::instance().par_end(), [&outputFile, c] (FitParameter& p) {
       auto parameter_graph = p.toTGraph();
       parameter_graph->SetName(p.getName().c_str());
       parameter_graph->SetTitle(p.getName().c_str());
       outputFile.WriteObject(parameter_graph, p.getName().c_str());

       parameter_graph->Draw("Al");
       c->Print("output.pdf", "pdf");
       c->Clear();
    });

    c->Print("output.pdf)", "pdf");
    return 0;
}
