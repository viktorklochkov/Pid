{

  auto pid_getter = new Pid::CutGGetter;

  /* negative pions */
  {
    auto cutg = new TCutG("cut_pion_neg", 15);
    cutg->SetVarX("hTrackdEdx_allTPC");
    cutg->SetVarY("");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetPoint(0, -1.19578, 1.27775);
    cutg->SetPoint(1, -1.57377, 1.31828);
    cutg->SetPoint(2, -2.09714, 1.18267);
    cutg->SetPoint(3, -3.34741, 1.30113);
    cutg->SetPoint(4, -4.95144, 1.457);
    cutg->SetPoint(5, -6.22594, 1.53493);
    cutg->SetPoint(6, -6.7057, 1.41491);
    cutg->SetPoint(7, -5.49419, 1.25437);
    cutg->SetPoint(8, -4.86421, 1.17644);
    cutg->SetPoint(9, -4.16154, 1.06421);
    cutg->SetPoint(10, -3.26018, 0.951991);
    cutg->SetPoint(11, -2.49451, 0.867824);
    cutg->SetPoint(12, -1.78215, 0.880293);
    cutg->SetPoint(13, -1.27816, 1.20917);
    cutg->SetPoint(14, -1.19578, 1.27775);

    int q = -1;// charge of the negative pion
    int n = cutg->GetN();
    double* xx = cutg->GetX();

    for (int i = 0; i < n; ++i) {
      xx[i] = q * TMath::Exp(xx[i] / q) / 20;
    }

    pid_getter->AddParticle(cutg, -211);
  }

  /* protons */
  {
    TCutG* cutg = new TCutG("cut_protons", 11);
    cutg->SetVarX("hTrackdEdx_allTPC");
    cutg->SetVarY("");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetPoint(0, 3.50396, 0.888116);
    cutg->SetPoint(1, 4.29527, 1.09983);
    cutg->SetPoint(2, 5.17111, 1.17644);
    cutg->SetPoint(3, 6.08754, 1.30145);
    cutg->SetPoint(4, 6.87546, 1.39823);
    cutg->SetPoint(5, 7.04792, 1.19056);
    cutg->SetPoint(6, 6.21942, 1.00506);
    cutg->SetPoint(7, 4.9885, 0.841742);
    cutg->SetPoint(8, 4.10928, 0.807465);
    cutg->SetPoint(9, 3.53778, 0.863921);
    cutg->SetPoint(10, 3.50396, 0.888116);

    int q = 1;// charge of the positive pion
    int n = cutg->GetN();
    double* xx = cutg->GetX();

    for (int i = 0; i < n; ++i) {
      xx[i] = q * TMath::Exp(xx[i] / q) / 20;
    }

    pid_getter->AddParticle(cutg, 2212);
  }

  {
    auto cutg = new TCutG("cut_pion", 16);
    cutg->SetVarX("hTrackdEdx_allTPC");
    cutg->SetVarY("");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetPoint(0, 1.59418, 1.27308);
    cutg->SetPoint(1, 2.12521, 1.13124);
    cutg->SetPoint(2, 2.93439, 1.19826);
    cutg->SetPoint(3, 3.38955, 1.17332);
    cutg->SetPoint(4, 3.72839, 1.26372);
    cutg->SetPoint(5, 5.80697, 1.48973);
    cutg->SetPoint(6, 6.18627, 1.49596);
    cutg->SetPoint(7, 6.11547, 1.35568);
    cutg->SetPoint(8, 5.68559, 1.32607);
    cutg->SetPoint(9, 4.11781, 1.11721);
    cutg->SetPoint(10, 3.72839, 1.09851);
    cutg->SetPoint(11, 3.46035, 0.970695);
    cutg->SetPoint(12, 2.44382, 0.858472);
    cutg->SetPoint(13, 1.63464, 0.983165);
    cutg->SetPoint(14, 1.41718, 1.178);
    cutg->SetPoint(15, 1.59418, 1.27308);

    int q = 1;// charge of the positive pion
    int n = cutg->GetN();
    double* xx = cutg->GetX();

    for (int i = 0; i < n; ++i) {
      xx[i] = q * TMath::Exp(xx[i] / q) / 20;
    }

    pid_getter->AddParticle(cutg, 211);
  }

  TFile ff("arsc_40agev_pid_graphical.root", "recreate");
  ff.cd();

  pid_getter->Write("pid_getter");
  pid_getter->Draw();
  ff.Close();
}
