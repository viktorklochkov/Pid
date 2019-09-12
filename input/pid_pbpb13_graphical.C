{

  // Example of making Pid::CutGGetter
  // PbPb @ 13A GeV/c NA61/SHINE

  //========= Macro generated from object: dEdx_pion_neg/Graph
  //========= by ROOT version6.14/08

  auto pid_getter = new Pid::CutGGetter;
  {
    TCutG *cutg = new TCutG("dEdx_pion_neg",16);
    cutg->SetVarX("hTrackdEdx_allTPC");
    cutg->SetVarY("");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetLineColor(2);
    cutg->SetLineWidth(4);
    cutg->SetPoint(0,-5.86112,1.47571);
    cutg->SetPoint(1,-5.48262,1.5025);
    cutg->SetPoint(2,-4.59666,1.43219);
    cutg->SetPoint(3,-3.54849,1.33174);
    cutg->SetPoint(4,-2.56271,1.23129);
    cutg->SetPoint(5,-1.81402,1.21455);
    cutg->SetPoint(6,-1.51454,1.315);
    cutg->SetPoint(7,-1.14436,1.10741);
    cutg->SetPoint(8,-1.72251,0.725714);
    cutg->SetPoint(9,-2.6875,0.662098);
    cutg->SetPoint(10,-3.63584,0.729062);
    cutg->SetPoint(11,-4.97933,0.94);
    cutg->SetPoint(12,-5.8528,1.25808);
    cutg->SetPoint(13,-6.03998,1.42549);
    cutg->SetPoint(14,-6.03998,1.42884);
    cutg->SetPoint(15,-5.86112,1.47571);


    int q = -1; // charge of the negative pion
    int n = cutg->GetN();
    double *xx = cutg->GetX();

    for (int i = 0; i < n; ++i) {
        xx[i] = q*TMath::Exp(xx[i]/q)/20;
    }

    pid_getter->AddParticle(cutg, -211);
  }

  {
    TCutG *cutg = new TCutG("dEdx_pion_pos_1",11);
    cutg->SetVarX("hTrackdEdx_allTPC");
    cutg->SetVarY("");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetLineColor(4);
    cutg->SetLineWidth(3);
    cutg->SetPoint(0,1.13211,1.09199);
    cutg->SetPoint(1,1.62046,1.28916);
    cutg->SetPoint(2,1.83632,1.19423);
    cutg->SetPoint(3,2.30698,1.15285);
    cutg->SetPoint(4,2.98996,1.2064);
    cutg->SetPoint(5,3.58447,0.631949);
    cutg->SetPoint(6,2.16897,0.624647);
    cutg->SetPoint(7,1.55676,0.73905);
    cutg->SetPoint(8,1.15334,1.01167);
    cutg->SetPoint(9,1.13919,1.09686);
    cutg->SetPoint(10,1.13211,1.09199);

    int q = 1; // charge of the positive pion
    int n = cutg->GetN();
    double *xx = cutg->GetX();

    for (int i = 0; i < n; ++i) {
        xx[i] = q*TMath::Exp(xx[i]/q)/20;
    }

    pid_getter->AddParticle(cutg, 211);
  }

  {
    TCutG *cutg = new TCutG("dEdx_pion_pos_2",8);
    cutg->SetVarX("hTrackdEdx_allTPC");
    cutg->SetVarY("");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetLineColor(4);
    cutg->SetLineWidth(3);
    cutg->SetPoint(0,3.5636,1.3);
    cutg->SetPoint(1,4.07238,1.10134);
    cutg->SetPoint(2,4.68541,1.20179);
    cutg->SetPoint(3,5.77803,1.38705);
    cutg->SetPoint(4,5.88646,1.52098);
    cutg->SetPoint(5,5.4319,1.54107);
    cutg->SetPoint(6,3.5636,1.29777);
    cutg->SetPoint(7,3.5636,1.3);
    int q = 1; // charge of the positive pion
    int n = cutg->GetN();
    double *xx = cutg->GetX();

    for (int i = 0; i < n; ++i) {
        xx[i] = q*TMath::Exp(xx[i]/q)/20;
    }

    pid_getter->AddParticle(cutg, 211);
  }

  {
    auto cutg = new TCutG("dEdx_proton_pos_1",8);
    cutg->SetVarX("hTrackdEdx_allTPC");
    cutg->SetVarY("");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetLineWidth(3);
    cutg->SetPoint(0,4.10142,1.09185);
    cutg->SetPoint(1,6.05205,1.34946);
    cutg->SetPoint(2,7.89315,1.13419);
    cutg->SetPoint(3,5.07152,0.675434);
    cutg->SetPoint(4,3.70503,0.636616);
    cutg->SetPoint(5,3.54335,0.904813);
    cutg->SetPoint(6,4.10663,1.08832);
    cutg->SetPoint(7,4.10142,1.09185);
    int q = 1; // charge of the positive pion
    int n = cutg->GetN();
    double *xx = cutg->GetX();

    for (int i = 0; i < n; ++i) {
        xx[i] = q*TMath::Exp(xx[i]/q)/20;
    }

    pid_getter->AddParticle(cutg, 2212);
  }




  TFile ff("pbpb_13agev_pid_graphical.root", "recreate");
  ff.cd();

  pid_getter->Write("pid_getter");
  pid_getter->Draw();
  ff.Close();

}
