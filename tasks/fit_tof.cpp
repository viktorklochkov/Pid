#include "RooRealVar.h"
#include "RooGlobalFunc.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooDataHist.h"
#include "RooPlot.h"

#include "TFile.h"
#include "TH2.h"

#include "Getter.h"
#include "ParticlePdf1D.h"
#include "ParticlePdf2D.h"

using namespace RooFit;

std::vector<Pid::ParticlePdf1D*> Fit1D(TH1* input, float momentum);

int main(int argc, char* argv[]){

  if(argc<=2){
    std::cout << "./fit_tof filename histoname" << std::endl;
    return 1;
  }

  TString filename{argv[1]};
  TString histoname{argv[2]};

  RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);

  auto* file{TFile::Open(filename)};
  TH2* m2p = file->Get<TH2>(histoname);
  m2p->Rebin2D(10, 1);

  auto* out_file{TFile::Open("result.root", "recreate")};

  for(int i=0; i<m2p->GetXaxis()->GetNbins(); ++i)
  {
    TH1* slice = m2p->ProjectionY(Form("px_%d",i), i, i);

    if(slice->GetEntries() < 1000) continue;
    const float momentum = m2p->GetXaxis()->GetBinCenter(i);

    auto result = Fit1D(slice, momentum);


  }

  Pid::ParticlePdf2DBinned p;
  Pid::Getter tof();

  out_file->Close();

}

std::vector<Pid::ParticlePdf1D*> Fit1D(TH1* input, float momentum){

  const float p2 = momentum*momentum;

  RooRealVar m2("m2","m^{2} (GeV^{2}/c^{2})",-0.5, 1.5);

  std::cout << "Momentum - " << momentum << std::endl;

  RooRealVar pi_width("pi_width", "pi_width", 0.005*p2, 0.003*p2, 0.01*p2);
  RooRealVar pi_mean ("pi_mean",  "pi_mean", 0.02, -0.01, 0.03);
  RooGaussian pi("pi","pi PDF", m2, pi_mean, pi_width);

  RooRealVar p_width("p_width", "p_width", 0.005*p2 + 0.01, 0.003*p2, 0.05*p2);
  RooRealVar p_mean ("p_mean",  "p_mean", 0.87, 0.84, 0.9);
  RooGaussian p("p","p PDF", m2, p_mean, p_width);

  RooRealVar K_width("K_width", "K_width", 0.005*p2, 0.003*p2, 0.01*p2);
  RooRealVar K_mean ("K_mean",  "K_mean", 0.24, 0.2, 0.27);
  RooGaussian K("K","K PDF", m2, K_mean, K_width);

  float pi_frac_max{1.0f};
  float K_frac_max{0.1f};
  if(momentum > 5.)
    K_frac_max = 0;
  if(momentum > 7.)
    pi_frac_max = 0;

  RooRealVar pi_frac("pi_frac","pi_frac", 0.7*pi_frac_max, 0., pi_frac_max);
  RooRealVar K_frac("K_frac","K_frac", 0.3*K_frac_max, 0.0, K_frac_max);
  RooAddPdf sum("sum","0.01 + pi+K+p",RooArgList(pi, K, p), RooArgList(pi_frac, K_frac));

  RooDataHist data("data","data", RooArgList(m2), Import(*input));
  sum.fitTo(data, PrintLevel(-1));

  RooPlot* m2frame = m2.frame();
  data.plotOn(m2frame);
  sum.plotOn(m2frame);
  sum.plotOn(m2frame, Components(pi), LineStyle(ELineStyle::kDashed), LineColor(kRed));
  sum.plotOn(m2frame, Components(p), LineStyle(ELineStyle::kDashed), LineColor(kMagenta));
  sum.plotOn(m2frame, Components(K), LineStyle(ELineStyle::kDashed), LineColor(kGreen+2));

  m2frame->Write(Form("bin_%f", momentum));

  return std::vector<Pid::ParticlePdf1D*>();
}