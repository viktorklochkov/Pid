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

  RooRealVar qp("qp","qp",1.,6) ;
  RooRealVar m2("m2","m2",0.5,1.2) ;

  RooRealVar mean_a("mean_a","mean_a", 0.02, 0.0, 0.05);
  RooRealVar mean_b("mean_b","mean_b", -0.01, -0.05, 0.);

  RooRealVar mean_kaon_a("mean_kaon_a","mean_kaon_a", 0.25, 0.23, 0.27);
  RooRealVar mean_kaon_b("mean_kaon_b","mean_kaon_b", -0.01, -0.05, 0.);

  RooRealVar mean_p_a("mean_p_a","mean_p_a", 0.9, 0.85, 0.95);
  RooRealVar mean_p_b("mean_p_b","mean_p_b", -0.01, -0.05, 0.);

  RooRealVar sigma_a("sigma_a","sigma_a",0.02, 0., 0.05);
  RooRealVar sigma_b("sigma_b","sigma_b", -0.01, -0.1, 0.1);
  RooRealVar sigma_c("sigma_c","sigma_c", 0.004, 0., 0.01);

  RooRealVar k("k","k", 1.5, 1., 2);
  RooRealVar k_kaon("k_kaon","k_kaon", 1.0, 0.7, 1.2);
  RooRealVar k_p("k_p","k_p", 0.5, 0.3, 0.7);

  RooFormulaVar mean("mean","mean_a+mean_b*qp", RooArgList(mean_a, mean_b, qp)) ;
  RooFormulaVar sigma("sigma","sigma_a + sigma_c*qp**2", RooArgList(sigma_a, sigma_c, qp)) ;
//  RooFormulaVar sigma("sigma","sigma_a + sigma_b*qp + sigma_c*qp**2", RooArgList(sigma_a, sigma_b, sigma_c, qp)) ;
  RooFormulaVar A("A","exp(-k*qp)", RooArgList(k, qp)) ;

  RooFormulaVar mean_kaon("mean_kaon","mean_kaon_a+mean_kaon_b*qp", RooArgList(mean_kaon_a, mean_kaon_b, qp)) ;
  RooFormulaVar A_kaon("A_kaon","exp(-k_kaon*qp)", RooArgList(k_kaon, qp)) ;

  RooFormulaVar mean_p("mean_p","mean_p_a+mean_p_b*qp", RooArgList(mean_p_a, mean_p_b, qp)) ;
  RooFormulaVar A_p("A_p","exp(-k_p*qp)", RooArgList(k_p, qp)) ;

  RooGenericPdf fit_kaon("fit_kaon","fit_kaon","A_kaon*exp(-(m2-mean_kaon)**2/2/sigma**2)",RooArgSet(A_kaon, mean_kaon, sigma, qp, m2)) ;
  RooGenericPdf fit_pion("fit_pion","fit_pion","A*exp(-(m2-mean)**2/2/sigma**2)",RooArgSet(A,mean,sigma, qp, m2)) ;
  RooGenericPdf fit_p("fit_p","fit_p","A_p*exp(-(m2-mean_p)**2/2/sigma**2)",RooArgSet(A_p,mean_p,sigma, qp, m2)) ;

  RooRealVar kaon_frac("kaon_frac","kaon_frac",0.05, 0.01, 0.1) ;
  RooRealVar pion_frac("pion_frac","pion_frac",0.4, 0.2, 0.7) ;

  RooAddPdf sum("sum","fit_pion+fit_kaon+fit_p",RooArgList(fit_kaon,fit_pion, fit_p), RooArgList(pion_frac, kaon_frac));

  RooDataHist dh("dh","dh", RooArgList(qp,m2), Import(*hpos));

  fit_p.fitTo(dh);

  TH2* data = qp.createHistogram("x vs y pdf", m2);
  dh.fillHistogram(data, RooArgList(qp, m2)) ;

  TH2* fit = qp.createHistogram("x vs y data",m2);
  fit_p.fillHistogram(fit, RooArgList(qp, m2)) ;

  const float scale = fit->GetMaximum() / data->GetMaximum();
  data->Scale(scale);

  TCanvas* c = new TCanvas("rf102_dataimport","rf102_dataimport",800,800);
  c->Divide(2,2);

  c->cd(1);
  data->Draw("colz") ;

  c->cd(2);
  fit->Draw("colz") ;

  c->cd(3);
  auto* temp = (TH2*) fit->Clone();
  temp->Divide(data);
  temp->Draw("colz");

  c->cd(4);
  RooPlot* xframe = m2.frame(Title("Gaussian p.d.f.")) ;
//  dh.plotOn(xframe, Cut("abs(qp-2)<0.01"));
  qp=2;
  sum.plotOn(xframe, Slice(qp));
  xframe->Draw();

  gPad->Update();

  TFile* f = TFile::Open("test.root", "recreate");

  temp->Write("ratio");
  fit->Write("fit");
  data->Write("data");
  c->Write("canvas");

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