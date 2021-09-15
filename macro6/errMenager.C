void errMenager()
{

  TFile* fileOut = TFile::Open("errMenager.root", "Recreate");

  TH2D   *hloader;
  int rebin =1;
  int dim=2;

  /*
  standard format for entry
  sl number of slices /projections
  edge is boundry of bins has number of slices-1
  parName is name of value that is checked (mass/momentum/time/etc)
  */


  ifstream optionFile;
  optionFile.open("ConfigurationFile.txt");

  if (optionFile.is_open())
  {
    optionFile>>dim;
    cout<<dim;
  }
  else cout << "Unable to open file";

  int sl[dim];
  double *edge[dim];
  TString parName[dim];

  /*
  double edge1[4]={25,26,27,28};
  edge[1]=edge1;
  double edge0[19]={5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95};
  edge[0]=edge0;
  */


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

  double total=0;
  TString StringA;
  TString StringB;
  TString FullName="";
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
  TFile *f1 = new TFile("folderBaseN4.root","READ");
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
    //cout<<FullName;
      for(int typ=0;typ<8;typ++)
      {

        switch(typ) {
          case 0 : hloader=f1->Get<TH2D>(FullName+"hAll");
          break;
          case 1 : hloader=f1->Get<TH2D>(FullName+"hProton");
          break;
          case 2 : hloader=f1->Get<TH2D>(FullName+"hPipos");
          break;
          case 3 : hloader=f1->Get<TH2D>(FullName+"hPineg");
          break;
          case 4 : hloader=f1->Get<TH2D>(FullName+"hKapos");
          break;
          case 5 : hloader=f1->Get<TH2D>(FullName+"hKaneg");
          break;
          case 6 : hloader=f1->Get<TH2D>(FullName+"hDeutron");
          break;
          case 7 : hloader=f1->Get<TH2D>(FullName+"hHe3");
          break;
        }
        for(int i=1;i<=2000/1;i++)
        {
          for(int j=1;j<=2000/1;j++)
          {
            if(hloader->GetBinError(i,j)<1)
            {
              hloader->SetBinError(i,j,1);
            }

          }
        }
        fileOut->mkdir(FullName);
        fileOut->cd(FullName);
        hloader->Write();
      }




  }


fileOut->Close();


}
