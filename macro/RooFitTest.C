#include "TH2.h"
#include "TFile.h"

TH2D* TestHisto();

void RooFitTest()
{

  gSystem->Load("libRooFit.so");
  gROOT->SetStyle("Plain");
  using namespace RooFit;

  std::unique_ptr <TFile> fIn { TFile::Open("../input/test_input.root", "read")};

// some test histograms
  std::unique_ptr <TH2D> hpion {(TH2D*) (fIn->Get("hTof_piplus_M2"))};
  std::unique_ptr <TH2D> hkaon {(TH2D*) (fIn->Get("hTof_kplus_M2"))};
  std::unique_ptr <TH2D> hproton {(TH2D*) (fIn->Get("hTof_p_M2"))};
  std::unique_ptr <TH2D> hpos {(TH2D*) (fIn->Get("hTofM2Pos"))};

  RooRealVar qp("qp","qp",1.0,8.) ;
  RooRealVar m2("m2","m2",-0.3, 1.2) ;

  qp.setBins(15);
  m2.setBins(50);

  RooRealVar sigma_a("sigma_a","sigma_a",1.31758e-03);
  RooRealVar sigma_c("sigma_c","sigma_c", 5.75105e-03);
  //  RooFormulaVar sigma("sigma","sigma_a + sigma_b*qp + sigma_c*qp**2", RooArgList(sigma_a, sigma_b, sigma_c, qp)) ;
  RooFormulaVar sigma("sigma","sigma_a + sigma_c*qp**2", RooArgList(sigma_a, sigma_c, qp)) ;

  //&&&&&&&&&&&&&&&& PIONS &&&&&&&&&&&&&&&&&&&&&&&

  RooRealVar mean_pion_a("mean_pion_a","mean_pion_a", 3.15130e-02);
  RooRealVar mean_pion_b("mean_pion_b","mean_pion_b", -9.29910e-03);

  RooRealVar k_pion("k_pion","k_pion", 2.13867);
  RooRealVar k_pion_a("k_pion_a","k_pion_a", -0.1044);

  RooRealVar pion_frac("pion_frac","pion_frac",0.3, 0.2, 0.7) ;

  RooFormulaVar mean("mean","mean_pion_a+mean_pion_b*qp", RooArgList(mean_pion_a, mean_pion_b, qp)) ;
  RooFormulaVar A("A","exp(-k_pion*qp - k_pion_a*qp**2)", RooArgList(k_pion, k_pion_a, qp)) ;
  RooGenericPdf fit_pion("fit_pion","fit_pion","A*exp(-(m2-mean)**2/2/sigma**2)",RooArgSet(A,mean,sigma, qp, m2)) ;

  //&&&&&&&&&&&&&&&& KAONS &&&&&&&&&&&&&&&&&&&&&&&

  RooRealVar mean_kaon_a("mean_kaon_a","mean_kaon_a", 2.60740e-01);
  RooRealVar mean_kaon_b("mean_kaon_b","mean_kaon_b", -1.07128e-02);

  RooRealVar k_kaon("k_kaon","k_kaon", 1.05359);
  RooRealVar kaon_frac("kaon_frac","kaon_frac",0.03, 0.01, 0.10) ;

  RooFormulaVar mean_kaon("mean_kaon","mean_kaon_a+mean_kaon_b*qp", RooArgList(mean_kaon_a, mean_kaon_b, qp)) ;
  RooFormulaVar A_kaon("A_kaon","exp(-k_kaon*qp)", RooArgList(k_kaon, qp)) ;
  RooGenericPdf fit_kaon("fit_kaon","fit_kaon","A_kaon*exp(-(m2-mean_kaon)**2/2/sigma**2)",RooArgSet(A_kaon, mean_kaon, sigma, qp, m2)) ;

  //&&&&&&&&&&&&&&&& PROTONS &&&&&&&&&&&&&&&&&&&&&&&

  RooRealVar mean_p_a("mean_p_a","mean_p_a", 0.909227);
  RooRealVar mean_p_b("mean_p_b","mean_p_b", -1.28760e-02);
  RooRealVar k_p("k_p","k_p", 6.96012e-01);

  RooRealVar sigma_p_a("sigma_p_a","sigma_p_a", 1.98167e-02);
  RooRealVar sigma_p_c("sigma_p_c","sigma_p_c", 5.08031e-03);

  RooFormulaVar sigma_p("sigma_p","sigma_p_a + sigma_p_c*qp**2", RooArgList(sigma_p_a, sigma_p_c, qp)) ;
  RooFormulaVar mean_p("mean_p","mean_p_a+mean_p_b*qp", RooArgList(mean_p_a, mean_p_b, qp)) ;
  RooFormulaVar A_p("A_p","exp(-k_p*qp)", RooArgList(k_p, qp)) ;
  RooGenericPdf fit_p("fit_p","fit_p","A_p*exp(-(m2-mean_p)**2/2/sigma_p**2)",RooArgSet(A_p,mean_p,sigma_p, qp, m2)) ;

  //&&&&&&&&&&&&&&&& TOTAL &&&&&&&&&&&&&&&&&&&&&&&
  RooAddPdf sum("sum","fit_pion+fit_kaon+fit_p",RooArgList(fit_pion, fit_kaon, fit_p), RooArgList(pion_frac, kaon_frac));

  RooDataHist dh("dh","dh", RooArgList(qp,m2), Import(*hpos));
  sum.fitTo(dh);

  //&&&&&&&&&&&&&&&& PLOT RESULTS &&&&&&&&&&&&&&&&&&&&&&&
  TFile* f = TFile::Open("test.root", "recreate");

  TH2* data = qp.createHistogram("x vs y pdf", m2);
  dh.fillHistogram(data, RooArgList(qp, m2)) ;

  TH2* fit = qp.createHistogram("x vs y data",m2);
  sum.fillHistogram(fit, RooArgList(qp, m2)) ;

  auto* temp = (TH2*) fit->Clone();
  temp->Divide(data);

  data->Write("data");
  fit->Write("fit");
  temp->Write("ratio");

  for(int i=0; i<15; ++i)
  {
    const float bin = 1.0 + (i+0.5)*0.5;
    auto sliceData = dh.reduce(RooArgSet(qp, m2), Form("abs(qp-%f)<0.1", bin) );
    RooPlot* xframe2 = m2.frame(40) ;
    sliceData->plotOn(xframe2) ;
    sum.plotOn(xframe2,ProjWData(*sliceData));
    xframe2->Write( Form("1D_%f", bin) );
  }

  f->Close();
}

TH2D* TestHisto()
{
  TH2D* histo = new TH2D("test_histo", "", 100,0,10,100,-5,5);

  for(int ix=0; ix<100; ++ix) {
    const int n_entries = 1000*(100+ix);
    TH1D temp_1d("temp", "", 200,-10,10);
    temp_1d.FillRandom("gaus", n_entries);

    for(int iy=0; iy<100; ++iy)
      histo->SetBinContent(ix+1, iy+1, temp_1d.GetBinContent( 25 + ix/2 + iy+1));
  }
//  histo->SaveAs("test_2Dhisto.root");
  return histo;
}


//1  k_pion       2.13867e+00   3.70507e-03   9.65023e-05  -4.45229e-01
//2  k_pion_a    -1.04465e-01   5.55659e-04   1.07261e-06  -2.08946e-02
//3  mean_pion_a   3.15130e-02   7.08720e-05   4.20145e-05   2.63563e-01
//4  mean_pion_b  -9.29910e-03   2.89374e-05   4.21009e-05  -1.87071e-01
//5  sigma_a      1.31758e-03   2.64752e-05   6.11796e-05  -1.34072e+00
//6  sigma_c      5.75105e-03   4.31143e-06   1.31208e-05  -4.46771e-01

//1  k_p          6.96012e-01   2.30190e-04   2.86654e-04   1.77084e+00
//2  mean_p_a     9.09227e-01   4.47530e-05   6.40369e-05   1.85596e-01
//3  mean_p_b    -1.28760e-02   1.56807e-05   1.00865e-05   5.06319e-01
//4  sigma_a      1.98167e-02   1.58737e-05   5.42917e-05  -6.48091e-01
//5  sigma_c      5.08031e-03   1.77452e-06   2.87553e-05  -5.22983e-01

//1  k_kaon       1.05359e+00   9.37530e-04   1.42434e-04   4.27219e-01
//2  mean_kaon_a   2.60740e-01   1.98520e-04   6.99477e-05   5.66879e-01
//3  mean_kaon_b  -1.07128e-02   8.22894e-05   2.31295e-05   6.08317e-01
//4  sigma_a      3.13617e-03   6.71185e-05   4.81467e-05  -1.21473e+00
//5  sigma_c      5.77212e-03   1.13721e-05   1.55420e-05  -4.44424e-01

//1  kaon_frac    4.78293e-02   2.23154e-05   1.40884e-04  -1.60030e-01
//2  pion_frac    2.86653e-01   4.42744e-05   6.56263e-05  -7.12054e-01
