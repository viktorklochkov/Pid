//
// Created by eugene on 16/05/2020.
//

#include <iostream>

#include <string>
#include <vector>
#include <algorithm>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH2.h>
#include <TStyle.h>
#include <RooWorkspace.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooCategory.h>
#include <RooFormulaVar.h>
#include <RooFunctor1DBinding.h>
#include <RooExtendPdf.h>
#include <RooAddPdf.h>
#include <RooPlot.h>
#include <TCanvas.h>
#include <RooSimultaneous.h>

#include <ROOT/RDataFrame.hxx>

#include <bb/BetheBlochShine.h>
#include <model/AsymmetricGaussianPDF.h>
#include <RooConstVar.h>


int main() {

    std::vector<double> ncl_binning{30., 35., 40., 45.0, 50., 55., 60., 70., 90., 110., 130., 150., 170., 200., 250.};
    TAxis ncl_axis(ncl_binning.size() - 1, ncl_binning.data());

    size_t min_entries_ncl = 2000ul;
    std::map<int, size_t> nn_ncl_bin;

    ROOT::RDataFrame rdf("negative/bin_103/track_tree",
                         "/home/eugene/STORAGE/data/pid/vtx_tack_dumps/pbpb_13agev_16_026/tracks_binned.root");
    auto rdf_ncl_binned = rdf
            .Define("ncl_bin", [ncl_axis](int ncl) -> int { return ncl_axis.FindBin(ncl); }, {"ncl"})
            .Define("sigma_factor", "TMath::Power(ncl, -0.5)");
    rdf_ncl_binned.Display(".*").GetValue().Print();
    std::cout << "NE = " << rdf_ncl_binned.Count().GetValue() << std::endl;

    rdf_ncl_binned.Foreach([&nn_ncl_bin](int ncl_bin) {
        auto insert_result = nn_ncl_bin.emplace(ncl_bin, 0);
        if (!insert_result.second) { /* map entry already exists */
            insert_result.first->second++;
        }
    }, {"ncl_bin"});


    auto rdf_filtered = rdf_ncl_binned.Filter([=](int ncl_bin) {
        return ncl_bin != -1 && nn_ncl_bin.at(ncl_bin) > min_entries_ncl;
    }, {"ncl_bin"}, "nn_cut");


    rdf_filtered.Display(".*").GetValue().Print();
    std::cout << "NE = " << rdf_filtered.Count().GetValue() << std::endl;

    /* make snapshot */
    std::string tmp_name = std::tmpnam(nullptr);
    tmp_name.append(".root");
    std::cout << tmp_name << std::endl;
    rdf_filtered.Snapshot("track_tree", tmp_name);

    /* prepare model */
    auto w = RooWorkspace("w");

    /* observables */
    RooRealVar v_dedx("dedx", "dE/dx", 0, 2.5, "MIP");
    RooCategory v_ncl_bin("ncl_bin", "");
    for (auto p: nn_ncl_bin) {
        if (p.second >= min_entries_ncl) v_ncl_bin.defineType(Form("ncl_bin_%d", p.first), p.first);
    }

    /* input data */
    RooDataSet ds("ds", "", RooArgSet(v_dedx, v_ncl_bin), RooFit::ImportFromFile(tmp_name.c_str(), "track_tree"));
    v_ncl_bin.Print();
    ds.Print();

    /* model parameters */
    RooRealVar p_delta("delta", "#delta", 0.079);
    RooRealVar p_sigma0_pion_neg("sigma0_pion_neg", "#sigma_0 (#pi^{-})", 0., 1., "MIP");
    RooRealVar p_yield_pion_neg("yield_pion_neg", "Y (#pi^{-})", 0., 1.);

    /* Bethe-Blochs */
    auto p_mid = 0.5 * (rdf_filtered.Min("p").GetValue() + rdf_filtered.Max("p").GetValue());
    RooConstVar bb_pion_neg("bb_pion_neg", "BB (#pi^{-})", BetheBlochAntoniMod(p_mid / 0.139));
    RooConstVar bb_electron("bb_electrons", "BB (e)", BetheBlochAntoniMod(p_mid / 0.000511));

    /* Derivatives */
    RooFormulaVar p_sigma0_electron("sigma0_electron", "#sigma_0 (e^{-})",
                                    "@0 * TMath::Power(@1 / @2, 0.65)",
                                    RooArgList(p_sigma0_pion_neg, bb_electron, bb_pion_neg));


    std::map<std::string, RooAbsPdf *> sim_fit_components;
    for (auto p : nn_ncl_bin) {
        if (p.second < min_entries_ncl) continue;

        auto ncl_bin = p.first;
        auto mean_sigma_factor = rdf_filtered
                .Filter([ncl_bin](int _ncl_bin) { return ncl_bin == _ncl_bin; }, {"ncl_bin"})
                .Mean("sigma_factor").GetValue();

        auto sigma_factor = new RooConstVar("sigma_factor", "", mean_sigma_factor);
        auto p_sigma_pion_neg = new RooFormulaVar("sigma_pion_neg", "#sigma (#pi^{-})",
                                       "@0 * @1",
                                       RooArgList(p_sigma0_pion_neg, *sigma_factor));

        auto p_sigma_electron = new RooFormulaVar("sigma_electron", "#sigma (e^{-})",
                                        "@0 * @1",
                                        RooArgList(p_sigma0_electron, *sigma_factor));

        auto pdf_pion_neg = new AsymmetricGaussianPDF(Form("pdf_pion_neg_%d", ncl_bin),"",v_dedx, bb_pion_neg, *p_sigma_pion_neg, p_delta);
        auto pdf_electron = new AsymmetricGaussianPDF(Form("pdf_electron_%d", ncl_bin),"",v_dedx, bb_electron, *p_sigma_electron, p_delta);

        auto composite_pdf = new RooAddPdf(Form("pdf_composite_%d", ncl_bin), "", *pdf_pion_neg, *pdf_electron, p_yield_pion_neg);
        composite_pdf->Print();

        sim_fit_components.emplace(std::string(Form("ncl_bin_%d", ncl_bin)), composite_pdf);
    }

    auto sim_pdf = new RooSimultaneous("sim_pdf", "", sim_fit_components, v_ncl_bin);
    sim_pdf->Print();

    sim_pdf->fitTo(ds, RooFit::Minos());


    auto c = new TCanvas;
    c->Print("fit_qa.pdf(", "pdf");

    for (const auto &st : v_ncl_bin) {
        std::cout << st->GetName() << std::endl;
        auto frame_dedx = v_dedx.frame();
        ds.plotOn(frame_dedx, RooFit::Cut(std::string("ncl_bin==ncl_bin::").append(st->GetName()).c_str()));
        sim_pdf->plotOn(frame_dedx, RooFit::Slice(v_ncl_bin, st->GetName()), RooFit::ProjWData(v_ncl_bin, ds));

        c->Clear();
        c->Divide(2,1);
        c->cd(1);
        frame_dedx->Draw();
        c->cd(2);
        gPad->SetLogy();
        frame_dedx->Draw();
        c->Print("fit_qa.pdf", "pdf");
    }

    auto frame_dedx = v_dedx.frame();
    ds.plotOn(frame_dedx);
    sim_pdf->plotOn(frame_dedx, RooFit::ProjWData(v_ncl_bin, ds));
    c->Clear();
    frame_dedx->Draw();
    c->Print("fit_qa.pdf", "pdf");




    c->Clear();
    c->Print("fit_qa.pdf)", "pdf");




    std::remove(tmp_name.c_str());
    return 0;
}
