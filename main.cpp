#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <iomanip>
#include <chrono>
#include <string>

#include <TSystem.h>
#include <TROOT.h>
#include "TH2.h"
#include "TFile.h"
#include "TMath.h"

#include "ParticleFit.h"
#include "Fitter.h"
#include "Getter.h"
#include "Constants.h"

TH2* cutTH2 (const std::unique_ptr <TH2D> &hFull, TCutG *cut)
{
  std::string name = hFull -> GetName ();
  name += "_cut";
  TH2 *hCut = (TH2*) hFull -> Clone (name.c_str());
  int nBinsX = hCut -> GetNbinsX ();
  int nBinsY = hCut -> GetNbinsY ();
  double x, y;
  
  for (int i = 1; i <= nBinsX; i++) 
  {
    for (int j = 1; j <= nBinsY; j++) 
    {
      x = hCut -> GetXaxis () -> GetBinCenter (i);
      y = hCut -> GetYaxis () -> GetBinCenter (j);
      if (!cut -> IsInside (x, y)) 
      {
        hCut -> SetBinContent (i, j, 0.);
        hCut -> SetBinError (i, j, 0.);
      }
    }
//    hCut -> Draw ("colz");
//    gPad -> SetLogz ();
//    cut -> SetLineColor (kRed);
//    cut -> Draw ("same");
  }
  return hCut;
}

int main(int argc, char **argv) {
  auto start = std::chrono::system_clock::now();
  ROOT::EnableImplicitMT(2);    

  std::unique_ptr <TFile> fIn { TFile::Open("../input/cbm_input_botvina_12agev.root", "read")};
  
// some test histograms  
  std::unique_ptr <TH2D> hpos {(TH2D*) fIn->Get("hTofM2")};
  std::unique_ptr <TH2D> hpionpos {(TH2D*) cutTH2 (hpos, (TCutG*) fIn->Get("piplus"))};
  std::unique_ptr <TH2D> hpionneg {(TH2D*) cutTH2 (hpos, (TCutG*) fIn->Get("piminus"))};
  std::unique_ptr <TH2D> hkaonpos {(TH2D*) cutTH2 (hpos, (TCutG*) fIn->Get("kaon"))};
  std::unique_ptr <TH2D> hkaonneg {(TH2D*) cutTH2 (hpos, (TCutG*) fIn->Get("kminus"))};
  std::unique_ptr <TH2D> hproton {(TH2D*) cutTH2 (hpos, (TCutG*) fIn->Get("proton"))};
// make reasonable bin size in momentum   
  hpionpos->Rebin2D(5,1);
  hkaonpos->Rebin2D(5,1);   
  hpionneg->Rebin2D(5,1);
  hkaonneg->Rebin2D(5,1);
  hproton->Rebin2D(5,1);
  hpos->Rebin2D(5,1);
// set a function which will be used for peak fitting (here, for example, Gaussian)
  TF1 fit ("fit", "[0]*exp(-0.5*((x-[1])/[2])**2)", -0.5, 1.5);   
  fit.SetParLimits(0, 0., 1e10);  
  fit.SetParLimits(2, 0., 1.);
  float m2;

  Pid::Fitter tof; // main object
  
  const float xmin=0.5;
  const float xmax=5.0  ;
  
// // // // // // // // // // // // // // // // //   
  Pid::ParticleFit pionpos( PidParticles::kPionPos );  

  m2 = TMath::Power(PidParticles::masses.at(PidParticles::kPionPos),2);
  fit.SetParLimits(1, m2 - 0.05, m2 + 0.05);
// set functions for parametrization of momentum dependance and some dummy starting values
  TF1 pionpos0 ("pionpos_A", "exp([0]+[1]*x)", 0.1, 10);       
  pionpos0.SetParameters(20., -2.);
  TF1 pionpos1 ("pionpos_mean", "pol2", 0.1, 10);              
  pionpos1.SetParameters(m2, 0, 0);
  TF1 pionpos2 ("pionpos_sigma", "pol2", 0.1, 10);             
  pionpos2.SetParameters(0.01, 0.0, 0);
  
  const std::vector <TF1> pionpos_par = { pionpos0, pionpos1, pionpos2 };
  pionpos.SetParametrization(pionpos_par);
  pionpos.SetFitFunction(fit);

  pionpos.SetRange( xmin, xmax );  // fitting range from xmin to xmax GeV/c
  pionpos.SetIsFitted();
  
  tof.AddParticle(pionpos, PidParticles::kPionPos); // add one particle to fitting procedure
  tof.SetHisto2D( std::move(hpionpos) );         // set input histogram
  tof.SetRangeX( xmin, xmax );                // set fitting range in X
  tof.SetRangeY( -0.1, 0.1 );                 // set fitting range in Y

  tof.SetOutputFileName("pionpos.root");
  tof.Fit();
  
  pionpos = tof.GetParticleSpecie(PidParticles::kPionPos);   // move ParticleFit back to our object. That is done to avoid pointers usage -> move semantics
  tof.Clear();
// // // // // // // // // // // // // // // // // // 


// // // // // // // // // // // // // // // // //   
  Pid::ParticleFit kaonpos( PidParticles::kKaonPos );
  
  m2 = TMath::Power(PidParticles::masses.at(PidParticles::kKaonPos),2);
  fit.SetParLimits(1, m2 - 0.05, m2 + 0.05);
  TF1 kaonpos0 ("kaonpos0", "exp([0]+[1]*x)", 0.1, 10);   
  kaonpos0.SetParameters(10., -1.);
  TF1 kaonpos1 ("kaonpos1", "pol2", 0.1, 10);             
  kaonpos1.SetParameters(m2, 0, 0);
  TF1 kaonpos2 ("kaonpos2", "pol2", 0.1, 10);             
  kaonpos2.SetParameters(0.01, 0.0, 0);
  
  const std::vector <TF1> kaonpos_par = { kaonpos0, kaonpos1, kaonpos2 };
  kaonpos.SetParametrization(kaonpos_par);
  kaonpos.SetFitFunction(fit);
  kaonpos.SetRange( xmin, xmax );
  kaonpos.SetIsFitted();
  
  tof.AddParticle(kaonpos, PidParticles::kKaonPos);
  tof.SetHisto2D( std::move(hkaonpos) );
  tof.SetRangeX( xmin, xmax );
  tof.SetRangeY( 0.2, 0.4 );
  tof.SetOutputFileName("kaonpos.root");
  tof.Fit();
  
  kaonpos = tof.GetParticleSpecie(PidParticles::kKaonPos);
  tof.Clear();
// // // // // // // // // // // // // // // // // // 

  
// // // // // // // // // // // // // // // // //   
  Pid::ParticleFit pionneg( PidParticles::kPionNeg );  

  m2 = TMath::Power(PidParticles::masses.at(PidParticles::kPionNeg),2);
  fit.SetParLimits(1, m2 - 0.05, m2 + 0.05);
// set functions for parametrization of momentum dependance and some dummy starting values
  TF1 pionneg0 ("pionneg_A", "exp([0]+[1]*x)", -10, -0.1);       
  pionneg0.SetParameters(20., 2.);
  TF1 pionneg1 ("pionneg_mean", "pol2", -10, -0.1);              
  pionneg1.SetParameters(m2, 0, 0);
  TF1 pionneg2 ("pionneg_sigma", "pol2", -10, -0.1);             
  pionneg2.SetParameters(0.01, 0.0, 0);
  
  const std::vector <TF1> pionneg_par = { pionneg0, pionneg1, pionneg2 };
  pionneg.SetParametrization(pionneg_par);
  pionneg.SetFitFunction(fit);

  pionneg.SetRange( -xmax, -xmin );  // fitting range from xmin to xmax GeV/c
  pionneg.SetIsFitted();
  
  tof.AddParticle(pionneg, PidParticles::kPionNeg); // add one particle to fitting procedure
  tof.SetHisto2D( std::move(hpionneg) );         // set input histogram
  tof.SetRangeX( -xmax, -xmin );                // set fitting range in X
  tof.SetRangeY( -0.1, 0.1 );                 // set fitting range in Y

  tof.SetOutputFileName("pionneg.root");
  tof.Fit();
  
  pionneg = tof.GetParticleSpecie(PidParticles::kPionNeg);   // move ParticleFit back to our object. That is done to avoid pointers usage -> move semantics
  tof.Clear();
// // // // // // // // // // // // // // // // // // 


// // // // // // // // // // // // // // // // //   
  Pid::ParticleFit kaonneg( PidParticles::kKaonNeg );
  
  m2 = TMath::Power(PidParticles::masses.at(PidParticles::kKaonNeg),2);
  fit.SetParLimits(1, m2 - 0.05, m2 + 0.05);
  TF1 kaonneg0 ("kaonneg0", "exp([0]+[1]*x)", -10, -0.1);   
  kaonneg0.SetParameters(10., 1.);
  TF1 kaonneg1 ("kaonneg1", "pol2", -10, -0.1);             
  kaonneg1.SetParameters(m2, 0, 0);
  TF1 kaonneg2 ("kaonneg2", "pol2", -10, -0.1);             
  kaonneg2.SetParameters(0.01, 0.0, 0);
  
  const std::vector <TF1> kaonneg_par = { kaonneg0, kaonneg1, kaonneg2 };
  kaonneg.SetParametrization(kaonneg_par);
  kaonneg.SetFitFunction(fit);
  kaonneg.SetRange( -xmax, -xmin );
  kaonneg.SetIsFitted();
  
  tof.AddParticle(kaonneg, PidParticles::kKaonNeg);
  tof.SetHisto2D( std::move(hkaonneg) );
  tof.SetRangeX( -xmax, -xmin );
  tof.SetRangeY( 0.2, 0.4 );
  tof.SetOutputFileName("kaonneg.root");
  tof.Fit();
  
  kaonneg = tof.GetParticleSpecie(PidParticles::kKaonNeg);
  tof.Clear();
// // // // // // // // // // // // // // // // // // 


// // // // // // // // // // // // // // // // //   
  Pid::ParticleFit proton( PidParticles::kProton );  
  
  m2 = TMath::Power(PidParticles::masses.at(PidParticles::kProton),2);
  fit.SetParLimits(1, m2 - 0.05, m2 + 0.05);
  TF1 proton0 ("proton0", "exp([0]+[1]*x+[2]*x*x)", 0.1, 10);   
  proton0.SetParameters(15., -1., 0);
  TF1 proton1 ("proton1", "pol2", 0.1, 10);             
  proton1.SetParameters( m2, 0, 0);
  TF1 proton2 ("proton2", "pol2", 0.1, 10);             
  proton2.SetParameters(0.1, 0.0, 0);
  
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
  Pid::ParticleFit bgpos( PidParticles::kBgPos );  
  
  TF1 bgpos0 ("bgpos0", "pol2", 0.1, 10);             
  bgpos0.SetParameters(100, 0, 0);
  TF1 bgpos1 ("bgpos1", "pol2", 0.1, 10);             
  bgpos1.SetParameters(0, 0, 0);
  TF1 bgpos2 ("bgpos2", "pol2", 0.1, 10);             
  bgpos2.SetParameters(0.0, 0.0, 0);
  
  const std::vector <TF1> bgpos_par = { bgpos0, bgpos1, bgpos2 };
  bgpos.SetParametrization(bgpos_par);
  bgpos.SetFitFunction( TF1("bgpos", "pol2", 0., 10) );
  bgpos.SetRange( xmin, xmax );
  bgpos.SetIsFitted();
// // // // // // // // // // // // // // // // //   
  

// // // // // // // // // // // // // // // // //   
  Pid::ParticleFit bgneg( PidParticles::kBgNeg );  
  
  TF1 bgneg0 ("bgneg0", "pol2", -10, -0.1);             
  bgneg0.SetParameters(100, 0, 0);
  TF1 bgneg1 ("bgneg1", "pol2", -10, -0.1);             
  bgneg1.SetParameters(0, 0, 0);
  TF1 bgneg2 ("bgneg2", "pol2", -10, -0.1);             
  bgneg2.SetParameters(0.0, 0.0, 0);
  
  const std::vector <TF1> bgneg_par = { bgneg0, bgneg1, bgneg2 };
  bgneg.SetParametrization(bgneg_par);
  bgneg.SetFitFunction( TF1("bgneg", "pol2", -10, 0) );
  bgneg.SetRange( -xmax, -xmin);
  bgneg.SetIsFitted();
// // // // // // // // // // // // // // // // //   
  

//  POSITIVE
  
  // fix mean and sigma and fit only integral
  proton.SetIsFixed( {false, true, true} );
  pionpos.SetIsFixed( {false, true, true} );
  kaonpos.SetIsFixed( {false, true, true} );
  // add all particles + background into the fit object
  tof.AddParticle(proton, PidParticles::kProton);
  tof.AddParticle(pionpos, PidParticles::kPionPos);
  tof.AddParticle(kaonpos, PidParticles::kKaonPos);
  tof.AddParticle(bgpos, PidParticles::kBgPos);
  
  tof.SetHisto2D( std::move(hpos) );
  tof.SetRangeX( xmin, xmax );
  tof.SetRangeY( -0.3, 1.3 );
  tof.SetOutputFileName("allpos.root");
  tof.Fit();
    
  proton = tof.GetParticleSpecie(PidParticles::kProton);
  pionpos = tof.GetParticleSpecie(PidParticles::kPionPos);
  kaonpos = tof.GetParticleSpecie(PidParticles::kKaonPos);
  bgpos = tof.GetParticleSpecie(PidParticles::kBgPos);
  
  tof.Clear();

  std::unique_ptr <TH2D> hpos1 {(TH2D*) (fIn->Get("hTofM2"))};
  hpos1->Rebin2D(5,1);
// release all parameters
  proton.SetIsFixed( {false, false, false} );
  pionpos.SetIsFixed( {false, false, false} );
  kaonpos.SetIsFixed( {false, false, false} );
  
  tof.AddParticle(proton, PidParticles::kProton);
  tof.AddParticle(pionpos, PidParticles::kPionPos);
  tof.AddParticle(kaonpos, PidParticles::kKaonPos);
  tof.AddParticle(bgpos, PidParticles::kBgPos);

  tof.SetHisto2D( std::move(hpos1) );
  tof.SetRangeX( xmin, xmax );
  tof.SetRangeY( -0.3, 1.3 );
  tof.SetOutputFileName("allpos_final.root");
  tof.Fit();
  
  proton = tof.GetParticleSpecie(PidParticles::kProton);
  pionpos = tof.GetParticleSpecie(PidParticles::kPionPos);
  kaonpos = tof.GetParticleSpecie(PidParticles::kKaonPos);
  bgpos = tof.GetParticleSpecie(PidParticles::kBgPos);  
  tof.Clear();

//  NEGATIVE
  
  std::unique_ptr <TH2D> hneg {(TH2D*) (fIn->Get("hTofM2"))};
  hneg->Rebin2D(5,1);
  // fix mean and sigma and fit only integral
  pionneg.SetIsFixed( {false, true, true} );
  kaonneg.SetIsFixed( {false, true, true} );
  // add all particles + background into the fit object
  tof.AddParticle(pionneg, PidParticles::kPionNeg);
  tof.AddParticle(kaonneg, PidParticles::kKaonNeg);
  tof.AddParticle(bgneg, PidParticles::kBgNeg);
  
  tof.SetHisto2D( std::move(hneg) );
  tof.SetRangeX( -xmax, -xmin );
  tof.SetRangeY( -0.3, 1.3 );
  tof.SetOutputFileName("allneg.root");
  tof.Fit();
    
  pionneg = tof.GetParticleSpecie(PidParticles::kPionNeg);
  kaonneg = tof.GetParticleSpecie(PidParticles::kKaonNeg);
  bgneg = tof.GetParticleSpecie(PidParticles::kBgNeg);
  
  tof.Clear();

  std::unique_ptr <TH2D> hneg1 {(TH2D*) (fIn->Get("hTofM2"))};
  hneg1->Rebin2D(5,1);
// release all parameters
  pionneg.SetIsFixed( {false, false, false} );
  kaonneg.SetIsFixed( {false, true, true} );
  
  tof.AddParticle(pionneg, PidParticles::kPionNeg);
  tof.AddParticle(kaonneg, PidParticles::kKaonNeg);
  tof.AddParticle(bgneg, PidParticles::kBgNeg);

  tof.SetHisto2D( std::move(hneg1) );
  tof.SetRangeX( -xmax, -xmin );
  tof.SetRangeY( -0.3, 1.3 );
  tof.SetOutputFileName("allneg_final.root");
  tof.Fit();
  
  pionneg = tof.GetParticleSpecie(PidParticles::kPionNeg);
  kaonneg = tof.GetParticleSpecie(PidParticles::kKaonNeg);
  bgneg = tof.GetParticleSpecie(PidParticles::kBgNeg);
    
  Pid::Getter getter;
  getter.AddParticle(proton, PidParticles::kProton);
  getter.AddParticle(pionpos, PidParticles::kPionPos);
  getter.AddParticle(kaonpos, PidParticles::kKaonPos);
  getter.AddParticle(bgpos, PidParticles::kBgPos);
  getter.AddParticle(pionneg, PidParticles::kPionNeg);
  getter.AddParticle(kaonneg, PidParticles::kKaonNeg);
  getter.AddParticle(bgneg, PidParticles::kBgNeg);

  std::unique_ptr <TFile> outfile{TFile::Open("pid_getter.root", "recreate")};
  getter.Write("pid_getter");
  outfile->Close();
  
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";
  return 0;
}
