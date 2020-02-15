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

  RooRealVar qp("qp","qp",1,5) ;
  RooRealVar m2("m2","m2",-0.2,0.2) ;

  RooRealVar mean_a("mean_a","mean_a", 0.02, 0.0, 0.1);
  RooRealVar mean_b("mean_b","mean_b", -0.01, -0.05, 0.);

  RooRealVar sigma_a("sigma_a","sigma_a",0.02, 0., 0.1);
  RooRealVar sigma_b("sigma_b","sigma_b", 0., -0.01, 0.01);
  RooRealVar sigma_c("sigma_c","sigma_c", 0.02, 0., 0.2);

  RooRealVar k("k","k", 1.5, 1., 2);

  RooFormulaVar mean("mean","mean_a+mean_b*qp", RooArgList(mean_a, mean_b, qp)) ;
  RooFormulaVar sigma("sigma","sigma_a+sigma_b*qp+sigma_c*qp**2", RooArgList(sigma_a, sigma_b, sigma_c, qp)) ;
  RooFormulaVar A("A","exp(-k*qp)", RooArgList(k,qp)) ;

  RooGenericPdf fit_func("fit_func","fit_func","A*exp(-(m2-mean)**2/2/sigma**2)",RooArgSet(A,mean,sigma, qp, m2)) ;

  RooPlot* xframe = m2.frame(Title("Gaussian p.d.f.")) ;

  RooDataHist dh("dh","dh", RooArgList(qp,m2), Import(*hpion));

  dh.plotOn(xframe);
  fit_func.fitTo(dh, "e");
  fit_func.plotOn(xframe);

  TH2* data = qp.createHistogram("x vs y pdf",m2);
  dh.fillHistogram(data, RooArgList(qp, m2)) ;

  TH2* fit = qp.createHistogram("x vs y data",m2);
  fit_func.fillHistogram(fit, RooArgList(qp, m2)) ;


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

  gPad->Update();
  c->SaveAs("test.root");

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