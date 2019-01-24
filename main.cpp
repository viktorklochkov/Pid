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
#include "TMath.h"

#include "ParticleFit.h"
#include "Fitter.h"
#include "Getter.h"
#include "Constants.h"

int main(int argc, char **argv) {
  auto start = std::chrono::system_clock::now();
  ROOT::EnableImplicitMT(2);    

  std::unique_ptr <TFile> fIn { TFile::Open("../input/test_input.root", "read")};
  
// some test histograms  
  std::unique_ptr <TH2D> hpion {(TH2D*) (fIn->Get("hTof_piplus_M2"))};
  std::unique_ptr <TH2D> hkaon {(TH2D*) (fIn->Get("hTof_kplus_M2"))};
  std::unique_ptr <TH2D> hproton {(TH2D*) (fIn->Get("hTof_p_M2"))};
  std::unique_ptr <TH2D> hpos {(TH2D*) (fIn->Get("hTofM2Pos"))};
// make reasonable bin size in momentum   
  hpion->Rebin2D(10,1);
  hkaon->Rebin2D(10,1);
  hproton->Rebin2D(10,1);
  hpos->Rebin2D(10,1);
// set a function which will be used for peak fitting (here, for example, Gaussian)
  TF1 fit ("fit", "[0]*exp(-0.5*((x-[1])/[2])**2)", -0.5, 1.5);   fit.SetParLimits(0, 0., 1e10);  fit.SetParLimits(2, 0., 1.);

  Pid::Fitter tof; // main object
  
  const float xmin=0.5;
  const float xmax=5;
  
// // // // // // // // // // // // // // // // //   
  Pid::ParticleFit pion( PidParticles::kPion );  

// set functions for parametrization of momentum dependance and some dummy starting values
  TF1 pion0 ("pion_A", "exp([0]+[1]*x)", 0.1, 10);       pion0.SetParameters(20., -2.);
  TF1 pion1 ("pion_mean", "pol2", 0.1, 10);              pion1.SetParameters( TMath::Power(PidParticles::masses[PidParticles::kPion],2), 0, 0);
  TF1 pion2 ("pion_sigma", "pol2", 0.1, 10);             pion2.SetParameters(0.01, 0.0, 0);
  
  const std::vector <TF1> pion_par = { pion0, pion1, pion2 };
  pion.SetParametrization(pion_par);
  pion.SetFitFunction(fit);

  pion.SetRange( xmin, xmax );  // fitting range from xmin to xmax GeV/c
  pion.SetIsFitted();
  
  tof.AddParticle(pion, PidParticles::kPion); // add one particle to fitting procedure
  tof.SetHisto2D( std::move(hpion) );         // set input histogram
  tof.SetRangeX( xmin, xmax );                // set fitting range in X
  tof.SetRangeY( -0.1, 0.1 );                 // set fitting range in Y

  tof.SetOutputFileName("pion.root");
  tof.Fit();
  
  pion = tof.GetParticleSpecie(PidParticles::kPion);   // move ParticleFit back to our object. That is done to avoid pointers usage -> move semantics
  tof.Clear();
// // // // // // // // // // // // // // // // // // 

// // // // // // // // // // // // // // // // //   
  Pid::ParticleFit kaon( PidParticles::kKaon );  
  
  TF1 kaon0 ("kaon0", "exp([0]+[1]*x)", 0.1, 10);   kaon0.SetParameters(10., -1.);
  TF1 kaon1 ("kaon1", "pol2", 0.1, 10);             kaon1.SetParameters( TMath::Power(PidParticles::masses[PidParticles::kKaon],2), 0, 0);
  TF1 kaon2 ("kaon2", "pol2", 0.1, 10);             kaon2.SetParameters(0.01, 0.0, 0);
  
  const std::vector <TF1> kaon_par = { kaon0, kaon1, kaon2 };
  kaon.SetParametrization(kaon_par);
  kaon.SetFitFunction(fit);
  kaon.SetRange( xmin, xmax );
  kaon.SetIsFitted();
  
  tof.AddParticle(kaon, PidParticles::kKaon);
  tof.SetHisto2D( std::move(hkaon) );
  tof.SetRangeX( xmin, xmax );
  tof.SetRangeY( 0.1, 0.4 );
  tof.SetOutputFileName("kaon.root");
  tof.Fit();
  
  kaon = tof.GetParticleSpecie(PidParticles::kKaon);
  tof.Clear();
// // // // // // // // // // // // // // // // // // 

// // // // // // // // // // // // // // // // //   
  Pid::ParticleFit proton( PidParticles::kProton );  
  
  TF1 proton0 ("proton0", "exp([0]+[1]*x+[2]*x*x)", 0.1, 10);   proton0.SetParameters(15., -1., 0);
  TF1 proton1 ("proton1", "pol2", 0.1, 10);             proton1.SetParameters( TMath::Power(PidParticles::masses[PidParticles::kProton],2), 0, 0);
  TF1 proton2 ("proton2", "pol2", 0.1, 10);             proton2.SetParameters(0.1, 0.0, 0);
  
  const std::vector <TF1> proton_par = { proton0, proton1, proton2 };
  proton.SetParametrization(proton_par);
  proton.SetFitFunction(fit);
  proton.SetRange( xmin, xmax );
  proton.SetIsFitted();
  
  tof.AddParticle(proton, PidParticles::kProton);
  tof.SetHisto2D( std::move(hproton) );
  tof.SetRangeX( xmin, xmax );
  tof.SetRangeY( 0.6, 1.1 );
  tof.SetOutputFileName("proton.root");
  tof.Fit();
  
  proton = tof.GetParticle(0);
  tof.Clear();
// // // // // // // // // // // // // // // // // // 

// // // // // // // // // // // // // // // // //   
  Pid::ParticleFit bg( PidParticles::kBg );  
  
  TF1 bg0 ("bg0", "pol2", 0.1, 10);             bg0.SetParameters(100, 0, 0);
  TF1 bg1 ("bg1", "pol2", 0.1, 10);             bg1.SetParameters(0, 0, 0);
  TF1 bg2 ("bg2", "pol2", 0.1, 10);             bg2.SetParameters(0.0, 0.0, 0);
  
  const std::vector <TF1> bg_par = { bg0, bg1, bg2 };
  bg.SetParametrization(bg_par);
  bg.SetFitFunction( TF1("bg", "pol2", 0., 10) );
  bg.SetRange( xmin, xmax );
  bg.SetIsFitted();
// // // // // // // // // // // // // // // // //   
  
  // fix mean and sigma and fit only integral
  proton.SetIsFixed( {false, true, true} );
  pion.SetIsFixed( {false, true, true} );
  kaon.SetIsFixed( {false, true, true} );
  // add all particles + background into the fit object
  tof.AddParticle(proton, PidParticles::kProton);
  tof.AddParticle(pion, PidParticles::kPion);
  tof.AddParticle(kaon, PidParticles::kKaon);
  tof.AddParticle(bg, PidParticles::kBg);
  
  tof.SetHisto2D( std::move(hpos) );
  tof.SetRangeX( xmin, xmax );
  tof.SetRangeY( -0.3, 1.3 );
  tof.SetOutputFileName("all.root");
  tof.Fit();
    
  proton = tof.GetParticleSpecie(PidParticles::kProton);
  pion = tof.GetParticleSpecie(PidParticles::kPion);
  kaon = tof.GetParticleSpecie(PidParticles::kKaon);
  bg = tof.GetParticleSpecie(PidParticles::kBg);
  
  tof.Clear();

  std::unique_ptr <TH2D> hpos1 {(TH2D*) (fIn->Get("hTofM2Pos"))};
  hpos1->Rebin2D(10,1);
// release all parameters
  proton.SetIsFixed( {false, false, false} );
  pion.SetIsFixed( {false, false, false} );
  kaon.SetIsFixed( {false, false, false} );
  
  tof.AddParticle(proton, PidParticles::kProton);
  tof.AddParticle(pion, PidParticles::kPion);
  tof.AddParticle(kaon, PidParticles::kKaon);
  tof.AddParticle(bg, PidParticles::kBg);

  tof.SetHisto2D( std::move(hpos1) );
  tof.SetRangeX( xmin, xmax );
  tof.SetRangeY( -0.3, 1.3 );
  tof.SetOutputFileName("all_final.root");
  tof.Fit();
  
  proton = tof.GetParticleSpecie(PidParticles::kProton);
  pion = tof.GetParticleSpecie(PidParticles::kPion);
  kaon = tof.GetParticleSpecie(PidParticles::kKaon);
  bg = tof.GetParticleSpecie(PidParticles::kBg);
    
  Pid::Getter getter;
  getter.AddParticle(proton, PidParticles::kProton);
  getter.AddParticle(pion, PidParticles::kPion);
  getter.AddParticle(kaon, PidParticles::kKaon);
  getter.AddParticle(bg, PidParticles::kBg);

  std::unique_ptr <TFile> outfile{TFile::Open("pid_getter.root", "recreate")};
  getter.Write("pid_getter");
  outfile->Close();
  
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";
  return 0;
}
