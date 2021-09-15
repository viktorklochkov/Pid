
void parmeterGraph2() {

  TString infoFileName = "ConfigurationFile.txt";
  int parNumber=6;
  int dim=2;
  ifstream optionFile;
  optionFile.open(infoFileName);
  if (optionFile.is_open())
  {
    optionFile>>dim;
    cout<<dim;
  }
  else cout << "Unable to open file";
  int sl[dim];
  double *edge[dim];
  TString parName[dim];
  if (optionFile.is_open())
  {
    for(int i=0;i<dim;i++)
    {
      optionFile>>sl[i];
      edge[i]=new double[sl[i]];
      for(int j=0;j<sl[i]-1;j++)
      {
        optionFile>>edge[i][j];
      }
      optionFile>>parName[i];
    }
  }
  TString StringA;
  TString StringB;
  TString FullName="";
  TString FullNameOut;
  int multi[dim]; //information of diference in indexing
  double storage[dim]; //place to store data for specific particle one at a time
  int filler[dim]; //place to store data about position in response to arrey of cuts
  multi[0]=1;
  for(int dm=1;dm<dim;dm++)
  {
    multi[dm]=multi[dm-1]*sl[dm-1]; //information of diference in indexing
  }
  int LoopN=1;
  int holder;
  for(int i=0;i<dim;i++)
  {
    LoopN=LoopN*sl[i]; //total number of histograms
  }

  TF1   *par0;
  TF1   *par1;
  TF1   *par2;
  TString outputFileName = "parameterGraph.root";
  TString inputFileName = "allpos.root";
  int colors [] = {kBlack,kGreen+2,kViolet,kOrange+2, kCyan, kYellow+3};

  TH2F* hPar[3][6];
  TH2F* hVal[6];

  int sll = 200;

  for(int g=0;g<6;g++)
  {
    for(int h=0;h<3;h++)
    {
      hPar[h][g]=new TH2F("hPar", "Single parameter; "+ parName[0]+"Bins; p (GeV/c);",15,-0.5,14.5,sll,0,12);
    }
    hVal[g]=new TH2F("hVal", "value; "+ parName[0]+"Bins; mass (GeV/c^{2})^{2} ;",15,-0.5,14.5,sll,0,6);
  }



  for (int a=0;a<LoopN;a++)
  {
    holder=a;
    FullName="";
    for(int i=0;i<dim;i++)
    {
      StringA = (Form("%d",holder%sl[i]));
      holder=holder/sl[i];
      FullName=FullName+parName[i]+StringA+"/";

    }
    FullName="exp2/"+FullName;
    cout<<FullName<<endl;
    TFile *fIn = TFile::Open(FullName+inputFileName, "read");

    TList *list = fIn -> GetListOfKeys();
    vector <TGraphErrors*> parList [3];
    vector <TH1D*> histList;
    vector <TF1*> f [3];
    vector <TF1> func;
    TGraphErrors *g;
    TCanvas c;
    c.SetLogy();
    for (auto key : *list)
    {
      TObject *obj = (TObject*) fIn -> Get(key->GetName());
      TString name = key->GetName();
      int ind;
      TString className = obj->ClassName();
      if(className.BeginsWith("TG"))
      {
        if (name == "chi2") continue;
        if (name.Contains("_0")) ind = 0;
        else if (name.Contains("_1")) ind = 1;
        else if (name.Contains("_2")) ind = 2;
        parList [ind].push_back ((TGraphErrors*) obj);
        parList [ind].back()->SetName(name);
        parList [ind].back()->SetTitle(name);
      }
      else if (className.BeginsWith("TH1"))
      {
        histList.push_back ((TH1D*) obj);
        histList.back()->SetName(name);
        histList.back()->SetTitle(name);

      }
    }

    float xmin = histList.at(0) -> GetXaxis() -> GetXmin();
    float xmax = histList.at(0) -> GetXaxis() -> GetXmax();

    for (int p = 0; p < parList[0].size(); p++)
    {
      TString name = parList[0].at(p)->GetName();
      TString formula = "gaus";
      if (name.Contains("bg")) formula = "pol2";
      name.Remove (name.Length() - 1);
      cout << name << endl;
      func.push_back (TF1 (name, formula, xmin, xmax));
      func [p].SetLineColor(colors [p]);
      for (int i = 0; i < 3; i++)
      {
        f [i].push_back (parList[i].at (p) -> GetFunction (name + Form("%d", i)));
        //cout << f [i].back()->GetName() << endl;
        //cout<<parList[i].at (p) -> GetFunction (name + Form("%d", i))<<endl;
      }
    }


/*
    for (auto hist : histList)
    {
      hist -> Draw ();
      TString sMom = hist->GetName();
      TList *funcList = hist -> GetListOfFunctions();
      float mom = sMom.Remove(0,2).Atof();
      //cout << mom << endl;
      for (int p = 0; p < parList[0].size(); p++)
      {
        if(f [1][p])
        {
          func [p].SetParameters(f [0][p]->Eval(mom), f [1][p]->Eval(mom), f [2][p]->Eval(mom));
          func [p].Draw("same");
          funcList -> Add (func [p].Clone(Form("%s_",func [p].GetName())));
        }
        else
        {
          cout<<"ERROR no entries in "<<a<<" "<<p<<endl;
        }
      }
      c.SetTitle(sMom);
      c.Write("c_" + sMom);
      hist->Write();
    }
*/
  if(a%15==5)
  {
    //int j=a;
    int j=a/15;


      for(int g=0;g<parNumber;g++)
      {
        for(int h=0;h<3;h++)
        {

          for(int l=0;l<sll;l++)
          {
            //hPar[h][g]->SetBinContent(j+1,l+1,f[h][g]->Eval((20./sll)*(l+1)));
            hPar[h][g]->Fill(hPar[h][g]->GetXaxis()->GetBinCenter(j+1),hPar[h][g]->GetYaxis()->GetBinCenter(l+1),f[h][g]->Eval((12./sll)*(l+1)));
          }
          hPar[h][g]->SetTitle(f[h][g]->GetName());
        }
      }
      for(int g=0;g<parNumber;g++)
      {


          for(int l=0;l<sll;l++)
          {
            func [g].SetParameters(f [0][g]->Eval(4), f [1][g]->Eval(4), f [2][g]->Eval(4));
            hVal[g]->SetBinContent(j+1,l+1,func[g].Eval((6./sll)*(l+1)));
          }
          hVal[g]->SetTitle(func[g].GetName());
      }
      //hPar->SetBinContent(j,l,j*l);


  }
  fIn->Close();


  }




  TFile *fOut = TFile::Open(outputFileName, "recreate");

  fOut->mkdir("parameterHsitogram");
  fOut->mkdir("ValueHsitogram");
  for(int g=0;g<6;g++)
  {
    for(int h=0;h<3;h++)
    {

      fOut->cd("parameterHsitogram");
      hPar[h][g]->Write();
    }

    fOut->cd("ValueHsitogram");
    hVal[g]->Write();

  }
  double suma;
  for(int j=0;j<15;j++)
  {


      for(int l=0;l<sll;l++)
      {
          suma=0;

          for(int g=0;g<5;g++)
          {
            suma=suma+hPar[0][g]->GetBinContent(j+1,l+1);
          }
          for(int g=0;g<6;g++)
          {
            hPar[0][g]->SetBinContent(j+1,l+1,hPar[0][g]->GetBinContent(j+1,l+1)/suma);
          }

      }


  }
  for(int j=0;j<15;j++)
  {


      for(int l=0;l<sll;l++)
      {
          suma=0;
          for(int g=0;g<5;g++)
          {
            suma=suma+ hVal[g]->GetBinContent(j+1,l+1);
          }
          for(int g=0;g<6;g++)
          {
            hVal[g]->SetBinContent(j+1,l+1,hVal[g]->GetBinContent(j+1,l+1)/suma);

          }

      }


  }


  fOut->mkdir("parameterHsitogramPur");
  fOut->mkdir("ValueHsitogramPur");
  for(int g=0;g<6;g++)
  {
    for(int h=0;h<3;h++)
    {

      fOut->cd("parameterHsitogramPur");
      hPar[h][g]->Write();
    }

    fOut->cd("ValueHsitogramPur");
    hVal[g]->Write();

  }

fOut->Close();
}
