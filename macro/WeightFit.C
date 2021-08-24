//Macro to create 2d distribution of purity

void WeightFit()
{
	int N= 7; //number of particle types
	TString partNames[N]; //list of names of these particles in previous file
	partNames[0]="pionpos__";
	partNames[1]="kaonpos__";
	partNames[2]="proton__";
	partNames[3]="deutron__";
	partNames[4]="he3__";
	partNames[5]="bgpos__";
	partNames[6]="fit1D";

	TFile *f = new TFile("fits.root"); //open file consisiting histograms with all fits
	TH1F * h1 = (TH1F*)f->Get("h_5.875000"); // get certain histogram
	//h1->Draw();
	//h1->GetFunction(partNames[0])->Draw();

	double mmin = -1.5;
	double mmax =  6.0; //Min and max value of m^2
	double mbin = 1000; //number of bins
	double mbinsize = (mmax-mmin)/mbin;


	TH1F **partHist= new TH1F*[N];
	TF1 **partFits= new TF1*[N];
	double sVal[N];
	double sSum;
	double sMax;

	for(int j=0;j<N;j++)
	{
		partHist[j]= new TH1F(partNames[j],partNames[j],mbin,mmin,mmax);	//histograms to store values
		partFits[j]=h1->GetFunction(partNames[j]);	//stored fits

	}


	for(int i=1;i<mbin+1;i++) //iterate by number of bins
	{
		sMax=0;

		for(int j=0;j<N;j++) //iterate for every particle
		{
			sVal[j]=partFits[j]->Eval(mmin+(i*mbinsize));

			//cout<< partFits[j]->Eval(mmin+(i*mbinsize))<<'\n';
			//cout<<j<<endl;
		}
		for(int j=0;j<5;j++) //iterate for every real particle !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		{
			partHist[j]->SetBinContent(i,sVal[j]/sVal[6]);
			if(sVal[j]>sMax)
			{
				sMax=sVal[j];
			}
			//cout<< partFits[j]->Eval(mmin+(i*mbinsize))<<'\n';
			//cout<<j<<endl;
		}
		partHist[6]->SetBinContent(i,sMax/sVal[6]);

	}


}
