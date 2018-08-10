#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <iomanip>
#include <chrono>

#include <TSystem.h>
#include <TROOT.h>
#include "TH2.h"
#include "TFile.h"

#include "ParticleFit.h"
#include "Fitter.h"


int main(int argc, char **argv) {
  auto start = std::chrono::system_clock::now();
  ROOT::EnableImplicitMT(2);    
  
  std::unique_ptr <TFile> fIn { TFile::Open("../input/test_input.root", "read")};
  std::unique_ptr <TH2D> hpion {(TH2D*) (fIn->Get("hTof_piplus_M2"))};
  
  hpion->Rebin2D(10,1);
  
  Pid::ParticleFit pion;  
  
  const std::vector <TF1> pion_par = { TF1("pion0", "pol1", 0, 10), TF1("pion1", "pol1", 0, 10), TF1("pion2", "pol2", 0, 10) };
  pion.SetParametrization(pion_par);
  pion.SetFitFunction( TF1("fit", "gaus", -0.5, 1.5) );
  pion.SetRange( 1., 5. );
  
  Pid::Fitter tof;
  tof.AddParticle(pion);
  tof.SetHisto2D( std::move(hpion) );
  tof.SetRange( 1., 5. );
  tof.Fit();
  
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";
  return 0;
}
