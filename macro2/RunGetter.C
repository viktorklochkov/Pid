#ifndef __CLING__

#include <TH2F.h>
#include <TFile.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <vector>
#include <map>
#include "Getter.h"

void RunGetter(TString InputFile);

int main(int argc, char**argv) {
  if (argc == 2) RunGetter (argv [1]);
  else RunGetter("pid_getter.root");
  return 1;
}

#endif // __CLING__

void RunGetter (TString InputFile = "pid_getter.root")
{

    TFile *f2{TFile::Open (InputFile)};        
    std::unique_ptr <Pid::Getter> getter {(Pid::Getter*) f2->Get("pid_getter")}; 
    std::unique_ptr<TRandom> r{new TRandom};
    
    std::vector <int> fittedPids = {PidParticles::kBgPos, PidParticles::kPionPos, PidParticles::kKaonPos, PidParticles::kProton, PidParticles::kHe3, PidParticles::kDeutron, PidParticles::kBgNeg, PidParticles::kPionNeg, PidParticles::kKaonNeg};
    
    std::map <int, TH2F*> h2Purity;
    for (auto pid : fittedPids)
    {
      h2Purity.emplace (pid, new TH2F (Form ("h2Purity_%d", pid), Form ("Purity for %d;q*p (GeV/#it{c});m^{2} (GeV^{2}/#it{c}^{4})", pid), 1000, -12, 20, 1000, -6., 6));
    }
    TAxis *xAxis = h2Purity.at(PidParticles::kBgPos)->GetXaxis();
    TAxis *yAxis = h2Purity.at(PidParticles::kBgPos)->GetYaxis();
    uint nBinsX = xAxis->GetNbins();
    uint nBinsY = yAxis->GetNbins();
    for (uint binX=1; binX<=nBinsX; binX++)
    {
      float qp = xAxis->GetBinCenter(binX);
      for (uint binY=1; binY<=nBinsY; binY++)
      {
        float m2 = yAxis->GetBinCenter(binY);
        auto prob = getter -> GetBayesianProbability (qp, m2);
        for (auto pid : fittedPids)
          h2Purity.at (pid)->Fill (qp, m2, prob.at(pid));
      }
    }
    TCanvas *c = new TCanvas ();
    c -> Divide (3, 3);
    int i = 0;
    for (auto pid : fittedPids)
    {
      c -> cd (++i);
      gPad -> SetLogz ();
      h2Purity.at(pid)->Draw("colz");
      h2Purity.at(pid)->GetYaxis()->SetTitleOffset(1.4);
    }
/*    
    for (int i=0; i<10; ++i)
    {
        const float m2 = r->Uniform(-0.1, 1);
        const float qp = r->Uniform(-5, 5);
        
        auto prob = getter->GetBayesianProbability(qp, m2);
        
        std::cout << "m2 = " << m2 << " qp = " << qp << " proton probability = " << prob[PidParticles::kProton] << std::endl;
        std::cout << "m2 = " << m2 << " qp = " << qp << " kaon   probability = " << prob[PidParticles::kKaonPos] << std::endl;
        std::cout << "m2 = " << m2 << " qp = " << qp << " pion   probability = " << prob[PidParticles::kPionPos] << std::endl;
        std::cout << "m2 = " << m2 << " qp = " << qp << " bg     probability = " << prob[PidParticles::kBg] << std::endl;
    }
    
    f2->Close();
*/
}
