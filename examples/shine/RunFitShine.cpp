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
    RooRealVar v_dEdx("x", "dEdx", 0, 2.5, "MIP");
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

    /* positive pions */
    RooRealVar v_pion_pos_bb("pion_pos_bb", "BB(#pi^{+})", 0, "MIP");
    RooRealVar v_pion_pos_sigma("pion_pos_sigma", "#sigma(#pi^{+})", 0, "MIP");
    RooRealVar v_pion_pos_n("pion_pos_n", "N(#pi^{+})", 0, "");
    AsymmetricGaussianPDF pdf_pion_pos("pion_pos", "", v_dEdx, v_pion_pos_bb, v_pion_pos_sigma, v_d);
    RooExtendPdf epdf_pion_pos("ext_pion_pos", "", pdf_pion_pos, v_pion_pos_n);

    /* protons model */
    RooRealVar v_proton_bb("proton_bb", "BB(p)", 0, "MIP");
    RooFormulaVar v_proton_sigma("proton_sigma", "#sigma(p)", "@0*TMath::Power(@1/@2, 0.65)",
                                   RooArgSet(v_pion_pos_sigma, v_proton_bb, v_pion_pos_bb));
    RooRealVar v_proton_n("proton_n", "N(p)", 0, "");
    AsymmetricGaussianPDF pdf_proton("proton", "", v_dEdx, v_proton_bb, v_proton_sigma, v_d);
    RooExtendPdf epdf_proton("ext_proton", "", pdf_proton, v_proton_n);

    /* positrons model */
    RooRealVar v_positron_bb("positron_bb", "BB(e^{-})", 0, "MIP");
    RooFormulaVar v_positron_sigma("positron_sigma", "#sigma(e^{-})", "@0*TMath::Power(@1/@2, 0.65)",
                                   RooArgSet(v_pion_pos_sigma, v_positron_bb, v_pion_pos_bb));
    RooRealVar v_positron_n("positron_n", "N(e^{-})", 0, "");
    AsymmetricGaussianPDF pdf_positron("positron", "", v_dEdx, v_positron_bb, v_positron_sigma, v_d);
    RooExtendPdf epdf_positron("ext_positron", "", pdf_positron, v_positron_n);

    /* kaons model */
    RooRealVar v_kaon_pos_bb("kaon_pos_bb", "BB(K^{+})", 0, "MIP");
    RooFormulaVar v_kaon_pos_sigma("kaon_pos_sigma", "#sigma(K^{+})", "@0*TMath::Power(@1/@2, 0.65)",
                                   RooArgSet(v_pion_pos_sigma, v_kaon_pos_bb, v_pion_pos_bb));
    RooRealVar v_kaon_pos_n("kaon_pos_n", "N(K^{+})", 0, "");
    AsymmetricGaussianPDF pdf_kaon_pos("kaon_pos", "", v_dEdx, v_kaon_pos_bb, v_kaon_pos_sigma, v_d);
    RooExtendPdf epdf_kaon_pos_pdf("ext_kaon_pos", "", pdf_kaon_pos, v_kaon_pos_n);

    std::list<ParticleFitModel> fit_models;

    /* negative */
    fit_models.emplace_back("pion_neg", epdf_pion_neg, dh_dEdx, "pion_neg_");
    fit_models.back().setRange(-5.5, -0.4);
    fit_models.back().parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(-211), -1));
    fit_models.back().parameter("sigma").fixTol(polyF({1.27477e-01, 1.05430e-02}), 0.05, -5.5, -4.);
    fit_models.back().parameter("sigma").fixTol(polyF({4.51838e-02, -3.28804e-02, 6.11775e-03, 6.08648e-03, 7.80633e-04}), 0.02, -4.0, -2.5);
    fit_models.back().parameter("sigma").fixTol(polyF({3.49191e-01, 3.19159e-01, 1.69915e-01, 4.49549e-02, 4.85014e-03}), 0.02, -2.5, -1);
    fit_models.back().parameter("sigma").fixTol(polyF({4.00148e-01, 4.12100e-01, 1.77967e-01}), 0.02, -1.0, -0.4);

    fit_models.emplace_back("kaon_neg", epdf_kaon_pdf, dh_dEdx, "kaon_neg_");
    fit_models.back().setRange(-5.3, -2.5);
    fit_models.back().parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(-321), -1));

    fit_models.emplace_back("electron", epdf_electron, dh_dEdx, "electron_");
    fit_models.back().setRange(-5., -0.4);
    fit_models.back().parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(-11), -1));

    /* positive */
    fit_models.emplace_back("pion_pos", epdf_pion_pos, dh_dEdx, "pion_pos_");
    fit_models.back().setRange(0.4, 5.5);
    fit_models.back().parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(211), 1));
    fit_models.back().parameter("sigma").fixTol(polyF({3.34221e-01, -2.10099e-01, 6.72043e-02, -9.48219e-03, 4.98327e-04}), 0.02, 1.15, 5.5);
    fit_models.back().parameter("sigma").fixTol(polyF({4.00148e-01, -4.12100e-01, 1.77967e-01}), 0.02, 0.4, 1.15);

    fit_models.emplace_back("proton", epdf_proton, dh_dEdx, "proton_");
    fit_models.back().setRange(2.2, 5.5);
    fit_models.back().parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(2212), 1));

    fit_models.emplace_back("positron", epdf_positron, dh_dEdx, "positron_");
    fit_models.back().setRange(0.4, 5.0);
    fit_models.back().parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(11), 1));

    fit_models.emplace_back("kaon_pos", epdf_kaon_pos_pdf, dh_dEdx, "kaon_pos_");
    fit_models.back().setRange(2.5, 5.5);
    fit_models.back().parameter("bb").fix(wrapToX(BetheBlochHelper::makeBBForPdg(321), 1));

    FitParameterRegistry::instance().par_find("d")->range(0., 0.3);


    /* IO */
    auto inputFile = TFile::Open(argv[1]);

    TH2D *inputHistogram = nullptr;
    inputFile->GetObject("reco_info/hTrackdEdx_allTPC", inputHistogram);
    inputHistogram->SetDirectory(nullptr);
    inputHistogram->Print();




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

        /* preparing input */
        {
            auto py = inputHistogram->ProjectionY("tmp", i, i);
            py->SetDirectory(nullptr);

            dh_dEdx.reset();
            RooDataHist slice_data("slice_dh", "", v_dEdx, py);
            dh_dEdx.add(slice_data);

            delete py;
        }

        auto slice_integral = dh_dEdx.sumEntries();

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

    }


    TFile outputFile("output.root", "RECREATE");
    std::for_each(FitParameterRegistry::instance().par_begin(), FitParameterRegistry::instance().par_end(), [&outputFile, c] (FitParameter& p) {
       p.dumpResult(outputFile, c);
       c->Print("output.pdf", "pdf");
       c->Clear();
    });

    c->Print("output.pdf)", "pdf");
    return 0;
}
