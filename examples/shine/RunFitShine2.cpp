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
#include <RooPlot.h>
#include <TCanvas.h>

#include <ROOT/RDataFrame.hxx>



int main() {

    TAxis axis_log_20p(20, 1.6, 7.);
    TAxis axis_pt(10, 0., 2.);
    TAxis axis_phi(8, -TMath::Pi(), TMath::Pi());

    auto n_bins = axis_log_20p.GetNbins() * axis_pt.GetNbins() * axis_phi.GetNbins();



    ROOT::RDataFrame rdf("track_tree","/home/eugene/STORAGE/data/pid/vtx_tack_dumps/pbpb_13agev_16_026/merged.vtxtd.root");

    auto rdf_10M = rdf
            .Range(0, 10000000)
            .Define("log_20p", "TMath::Log(20*p)")
            /* side (RST/WST) */
            .Define("side", [] (const int charge, const double phi) -> int {
                return (
                        (charge > 0 && -TMath::PiOver2() < phi && phi < TMath::PiOver2()) ||
                        (charge < 0 && phi < -TMath::PiOver2() && TMath::PiOver2() < phi)
                )? 1 : -1;
            }, {"charge", "phi"})
            .Define("bin_log_20p", [axis_log_20p] (double log_20p) -> int {
                return axis_log_20p.FindBin(log_20p);
            }, {"log_20p"})
            .Define("bin_pt", [axis_pt] (double pt) -> int {
                return axis_pt.FindBin(pt);
            }, {"pt"})
            .Define("bin_phi", [axis_phi] (double phi) -> int {
                return axis_phi.FindBin(phi);
            }, {"phi"})
            .Define("bin_global", [=] (int i_log_20p, int i_pt, int i_phi) -> int {
                return i_log_20p + axis_log_20p.GetNbins() * i_pt + axis_log_20p.GetNbins() * axis_pt.GetNbins() * i_phi;
            }, {"bin_log_20p", "bin_pt", "bin_phi"})
            .Define("denom_sqrt_ncl", "1.0/TMath::Sqrt(ncl)")
            ;

    for (int ib = 0; ib < n_bins; ++ib) {
        auto filtered = rdf_10M.Filter([ib] (const int bin_global) { return ib == bin_global; }, {"bin_global"}, Form("bin_%d", ib));
    }

    for (auto &report : rdf_10M.Report().GetValue()) {
        std::cout << "Passed " << report.GetPass() << "out of" << report.GetAll() << std::endl;
    }

    {
        auto d = rdf_10M.Display("");
        d->Print();
    }




    /* control plots */
    auto h_phi = rdf_10M.Histo1D({"h_phi", "#Phi", 600, -TMath::Pi(), +TMath::Pi()}, "phi");
    auto h_phi_pos = rdf_10M.Filter("charge == 1").Histo1D({"h_phi_pos", "#Phi (positive)", 600, -TMath::Pi(), +TMath::Pi()}, "phi");
    auto h_phi_neg = rdf_10M.Filter("charge == -1").Histo1D({"h_phi_neg", "#Phi (negative)", 600, -TMath::Pi(), +TMath::Pi()}, "phi");

    auto h_pt = rdf_10M.Histo1D({"h_pt", "p_{T}", 1000, 0., 10.}, "pt");

    auto h_dedx_p_neg = rdf_10M.Filter("charge == -1").Histo2D({"h_dedx_p_neg", "dE/dx vs momentum (-)", 1000, 0., 20., 1000, 0., 2.5}, "p", "dedx");
    auto h_dedx_p_pos = rdf_10M.Filter("charge == 1").Histo2D({"h_dedx_p_pos", "dE/dx vs momentum (+)", 1000, 0., 20., 1000, 0., 2.5}, "p", "dedx");
    auto h_dedx_logp_neg = rdf_10M.Filter("charge == -1").Histo2D({"h_dedx_logp_neg", "dE/dx vs Log(20 x p) (-)", 1000, 0., 7., 1000, 0., 2.5}, "log_20p", "dedx");
    auto h_dedx_logp_pos = rdf_10M.Filter("charge == 1").Histo2D({"h_dedx_logp_pos", "dE/dx vs Log(20 x p) (+)", 1000, 0., 7., 1000, 0., 2.5}, "log_20p", "dedx");

    auto h_ncl = rdf_10M.Histo1D({"h_ncl", "N_{cl}", 300, 0, 300}, "ncl");
    auto h_ncl_phi = rdf_10M.Histo2D({"h_ncl_phi", "N_{cl} vs #Phi", 600, -TMath::Pi(), TMath::Pi(), 300, 0, 300}, "phi", "ncl");
    auto h_ncl_p = rdf_10M.Histo2D({"h_ncl_p", "N_{cl} vs p", 1000, 0, 20, 300, 0, 300}, "p", "ncl");

    auto h_denom_sqrt_ncl = rdf_10M.Histo1D({"h_denom_sqrt_ncl", "N_{cl}^{-1/2}", 1000, 0, 0.2}, "denom_sqrt_ncl");

    auto h_bin_global = rdf_10M.Histo1D({"h_bin_global", "BIN", n_bins, 0, double(n_bins)}, "bin_global");

    auto c = new TCanvas;
    gStyle->SetOptStat(0);
    c->Print("output.pdf(", "pdf");


    h_phi->SetLineColor(kBlack);
    h_phi->Draw();
    h_phi_pos->SetLineColor(kRed);
    h_phi_pos->Draw("same");
    h_phi_neg->Draw("same");
    gPad->BuildLegend();
    c->Print("output.pdf", "pdf");
    c->Clear();

    gPad->SetLogy();
    h_pt->Draw();
    c->Print("output.pdf", "pdf");
    c->Clear();
    gPad->SetLogy(0);

    gPad->SetLogz(1);
    h_dedx_p_neg->Draw("colz");
    c->Print("output.pdf", "pdf");
    c->Clear();

    gPad->SetLogz(1);
    h_dedx_p_pos->Draw("colz");
    c->Print("output.pdf", "pdf");
    c->Clear();


    gPad->SetLogz(1);
    h_dedx_logp_neg->Draw("colz");
    c->Print("output.pdf", "pdf");
    c->Clear();

    h_dedx_logp_pos->Draw("colz");
    c->Print("output.pdf", "pdf");
    c->Clear();

    h_ncl->Draw();
    c->Print("output.pdf", "pdf");
    c->Clear();

    h_ncl_phi->Draw("colz");
    c->Print("output.pdf", "pdf");
    c->Clear();

    h_ncl_p->Draw("colz");
    c->Print("output.pdf", "pdf");
    c->Clear();

    h_denom_sqrt_ncl->Draw();
    c->Print("output.pdf", "pdf");
    c->Clear();

    gPad->SetLogy();
    h_bin_global->Draw();
    c->Print("output.pdf", "pdf");
    c->Clear();

    c->Print("output.pdf)", "pdf");

    return 0;
}
