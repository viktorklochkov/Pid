#ifndef __CLING__

#include <TH2F.h>
#include <TFile.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <vector>
#include <map>
#include "Getter.h"

void RunGetter(TString InputFile);

int main(int argc, char **argv) {
  if (argc == 2) RunGetter(argv[1]);
  else RunGetter("../build/pid_getter.root");
  return 1;
}

#endif // __CLING__

void RunGetter(TString InputFile = "../build/pid_getter.root") {

  TFile *f2{TFile::Open(InputFile)};
  std::unique_ptr<Pid::Getter> getter{(Pid::Getter *) f2->Get("pid_getter")};
  std::unique_ptr<TRandom> r{new TRandom};

  std::vector<int> fittedPids =
      {PidParticles::kBgPos, PidParticles::kPionPos, PidParticles::kKaonPos, PidParticles::kProton,
       PidParticles::kBgNeg, PidParticles::kPionNeg, PidParticles::kKaonNeg};

  std::map<int, TH2F *> h2Purity;
  for (auto pid : fittedPids) {
    h2Purity.emplace(pid,
                     new TH2F(Form("h2Purity_%d", pid),
                              Form("Purity for %d;q*p (GeV/#it{c});m^{2} (GeV^{2}/#it{c}^{4})", pid),
                              1000,
                              -10,
                              10,
                              1000,
                              -1.,
                              4.));
  }
  for (float qp = -10.; qp <= 10.; qp += 0.02)
    for (float m2 = -1.; m2 <= 4.; m2 += 0.005) {
      auto prob = getter->GetBayesianProbability(qp, m2);
      for (auto pid : fittedPids)
        h2Purity.at(pid)->Fill(qp, m2, prob.at(pid));
    }

  TCanvas *c = new TCanvas();
  c->Divide(4, 2);
  int i = 0;
  for (auto pid : fittedPids) {
    c->cd(++i);
    gPad->SetLogz();
    h2Purity.at(pid)->Draw("colz");
    h2Purity.at(pid)->GetYaxis()->SetTitleOffset(1.4);
  }
}
