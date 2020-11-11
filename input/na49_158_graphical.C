{
  auto pid_getter = new Pid::CutGGetter;
  {
    TCutG *cutg = new TCutG("pi_neg",23);
    cutg->SetVarX("hTrackdEdx_allTPC");
    cutg->SetVarY("");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetPoint(0,-8.545442,1.765711);
    cutg->SetPoint(1,-7.311222,1.755042);
    cutg->SetPoint(2,-6.126702,1.643025);
    cutg->SetPoint(3,-4.958748,1.504338);
    cutg->SetPoint(4,-4.097279,1.466999);
    cutg->SetPoint(5,-3.608561,1.418992);
    cutg->SetPoint(6,-2.929325,1.360316);
    cutg->SetPoint(7,-2.548291,1.242965);
    cutg->SetPoint(8,-2.37434,1.077607);
    cutg->SetPoint(9,-2.366057,0.8642411);
    cutg->SetPoint(10,-2.457174,0.704217);
    cutg->SetPoint(11,-2.672541,0.5922001);
    cutg->SetPoint(12,-2.871342,0.5608454);
    cutg->SetPoint(13,-3.219243,0.6882146);
    cutg->SetPoint(14,-3.48431,0.8777536);
    cutg->SetPoint(15,-4.097279,1.056014);
    cutg->SetPoint(16,-4.842781,1.189624);
    cutg->SetPoint(17,-5.712534,1.30164);
    cutg->SetPoint(18,-6.582286,1.386987);
    cutg->SetPoint(19,-7.634273,1.509672);
    cutg->SetPoint(20,-8.603426,1.669696);
    cutg->SetPoint(21,-8.644843,1.728372);
    cutg->SetPoint(22,-8.545442,1.765711);

    int q = -1; // charge of the negative pion
    int n = cutg->GetN();
    double *xx = cutg->GetX();

    for (int i = 0; i < n; ++i) {
        xx[i] = q*TMath::Exp(xx[i]/q)/20;
    }

    pid_getter->AddParticle(cutg, -211);
  }

  {
    TCutG *cutg = new TCutG("pi_pos_1",10);
    cutg->SetVarX("hTrackdEdx_allTPC");
    cutg->SetVarY("");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetPoint(0,2.846912,1.296997);
    cutg->SetPoint(1,3.535986,0.7490097);
    cutg->SetPoint(2,3.245412,0.4420048);
    cutg->SetPoint(3,2.929932,0.3627778);
    cutg->SetPoint(4,1.825753,0.4254992);
    cutg->SetPoint(5,1.767639,0.7886232);
    cutg->SetPoint(6,2.033306,1.092327);
    cutg->SetPoint(7,2.572942,1.280491);
    cutg->SetPoint(8,2.797099,1.3069);
    cutg->SetPoint(9,2.846912,1.296997);

    int q = 1; // charge of the positive pion
    int n = cutg->GetN();
    double *xx = cutg->GetX();

    for (int i = 0; i < n; ++i) {
        xx[i] = q*TMath::Exp(xx[i]/q)/20;
    }

    pid_getter->AddParticle(cutg, 211);
  }
  
  {
    TCutG *cutg = new TCutG("pi_pos_2",21);
    cutg->SetVarX("hTrackdEdx_allTPC");
    cutg->SetVarY("");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetPoint(0,3.535986,1.396031);
    cutg->SetPoint(1,4.073913,1.482641);
    cutg->SetPoint(2,4.69828,1.521473);
    cutg->SetPoint(3,5.218478,1.55913);
    cutg->SetPoint(4,5.88548,1.604002);
    cutg->SetPoint(5,6.348913,1.657188);
    cutg->SetPoint(6,7.330978,1.678454);
    cutg->SetPoint(7,7.394565,1.675395);
    cutg->SetPoint(8,7.663044,1.672335);
    cutg->SetPoint(9,7.677174,1.623382);
    cutg->SetPoint(10,7.313441,1.534678);
    cutg->SetPoint(11,6.665877,1.442246);
    cutg->SetPoint(12,5.819063,1.356417);
    cutg->SetPoint(13,4.980551,1.244179);
    cutg->SetPoint(14,4.432612,1.122037);
    cutg->SetPoint(15,3.96087,1.072657);
    cutg->SetPoint(16,3.834861,1.158349);
    cutg->SetPoint(17,3.635611,1.300298);
    cutg->SetPoint(18,3.560892,1.399332);
    cutg->SetPoint(19,3.569194,1.402633);
    cutg->SetPoint(20,3.535986,1.396031);
    
    int q = 1; // charge of the positive pion
    int n = cutg->GetN();
    double *xx = cutg->GetX();

    for (int i = 0; i < n; ++i) {
        xx[i] = q*TMath::Exp(xx[i]/q)/20;
    }

    pid_getter->AddParticle(cutg, 211);
  }

  {
    TCutG * cutg = new TCutG("proton_1",14);
    cutg->SetVarX("hTrackdEdx_allTPC");
    cutg->SetVarY("");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetPoint(0,2.942279,1.916661);
    cutg->SetPoint(1,3.242129,1.987112);
    cutg->SetPoint(2,2.493939,3.93556);
    cutg->SetPoint(3,2.251679,5.800737);
    cutg->SetPoint(4,2.084963,7.299851);
    cutg->SetPoint(5,1.97816,9.531091);
    cutg->SetPoint(6,1.592627,9.653112);
    cutg->SetPoint(7,1.631702,7.352146);
    cutg->SetPoint(8,1.785394,5.103474);
    cutg->SetPoint(9,1.97295,3.186002);
    cutg->SetPoint(10,2.129247,2.227266);
    cutg->SetPoint(11,2.117691,1.821552);
    cutg->SetPoint(12,2.698651,1.88848);
    cutg->SetPoint(13,2.942279,1.916661);
 
    int q = 1; // charge of the positive pion
    int n = cutg->GetN();
    double *xx = cutg->GetX();

    for (int i = 0; i < n; ++i) {
        xx[i] = q*TMath::Exp(xx[i]/q)/20;
    }

    pid_getter->AddParticle(cutg, 2212);
  }

  {
    TCutG *cutg = new TCutG("proton_2",20);
    cutg->SetVarX("hTrackdEdx_allTPC");
    cutg->SetVarY("");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetPoint(0,3.98913,1.066538);
    cutg->SetPoint(1,4.420109,1.115491);
    cutg->SetPoint(2,5.006522,1.243994);
    cutg->SetPoint(3,5.790761,1.34496);
    cutg->SetPoint(4,6.758696,1.445926);
    cutg->SetPoint(5,7.338044,1.531594);
    cutg->SetPoint(6,7.521739,1.574428);
    cutg->SetPoint(7,7.88913,1.620322);
    cutg->SetPoint(8,8.030435,1.583607);
    cutg->SetPoint(9,8.136413,1.430628);
    cutg->SetPoint(10,7.952717,1.302126);
    cutg->SetPoint(11,7.53587,1.103253);
    cutg->SetPoint(12,6.525544,0.9625121);
    cutg->SetPoint(13,5.74837,0.8034138);
    cutg->SetPoint(14,4.533152,0.5005153);
    cutg->SetPoint(15,3.967935,0.5035749);
    cutg->SetPoint(16,3.572283,0.5127536);
    cutg->SetPoint(17,3.551087,0.772818);
    cutg->SetPoint(18,4.010326,1.066538);
    cutg->SetPoint(19,3.98913,1.066538);
 
    int q = 1; // charge of the positive pion
    int n = cutg->GetN();
    double *xx = cutg->GetX();

    for (int i = 0; i < n; ++i) {
        xx[i] = q*TMath::Exp(xx[i]/q)/20;
    }

    pid_getter->AddParticle(cutg, 2212);
  }

  TFile ff("na49_158agev_pid_graphical.root", "recreate");
  ff.cd();

  pid_getter->Write("pid_getter");
  pid_getter->Draw();
  ff.Close();

}
