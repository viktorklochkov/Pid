//Macro to create 2d distribution of purity

void WeightFit2D()
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
	
	
	double mmin = -1.5;
	double mmax =  6.0; //Min and max value of m^2 
	double mbin = 1000; //number of bins 
	double mbinsize = (mmax-mmin)/mbin;
	
	
	TH2F **partHist= new TH2F*[N];
	TH2F **partHistReal= new TH2F*[N];
	TF1 **partFits= new TF1*[N];
	double sVal[N]; //storage for loop
	double sSum; //sum of sVal
	double sMax; // storage for max
	double momentum;
	int k=0;
	
	
	//double massStart = 0.375;
	TH1F * h1; 
	vector <TH1F*> hList;

	TFile *f = new TFile("fits.root"); //open file consisiting histograms with all fits
	TList *list = f -> GetListOfKeys();
	for (auto key : *list)
  	{
   	 	TObject *obj = (TObject*) f -> Get(key->GetName());
    	 	TString name = key->GetName();
    	 	//cout<<name<<endl;
    		TString className = obj->ClassName();
    		if(className.BeginsWith("TH1"))
    		{
			hList.push_back ((TH1F*) obj);
    			//h1 = (TH1F*)f->Get(name);
    			momentum=atof(name.Replace(0,2,"",0));
    			cout<<momentum<<endl;
			
    		}
    	}
    	
	 // get certain histogram
	//h1->Draw();
	//h1->GetFunction(partNames[0])->Draw();
	
	
	int pbin=hList.size();
	double pmin=atof(hList[0]->GetName()+2);
	double pmax=atof(hList[hList.size()-1]->GetName()+2);;
	
	for(int j=0;j<N;j++)
	{
		partHist[j]= new TH2F(partNames[j],partNames[j],mbin,mmin,mmax,hList.size(),0,20);	//histograms to store values
		partHistReal[j]= new TH2F(partNames[j]+2,partNames[j]+2,mbin,mmin,mmax,pbin,pmin ,pmax);
		
		partHist[j]->GetXaxis()->SetTitle("m^2 [GeV]");
   		partHist[j]->GetYaxis()->SetTitle("p [GeV]");
   		partHistReal[j]->GetXaxis()->SetTitle("m^2 [GeV]");
   		partHistReal[j]->GetYaxis()->SetTitle("p [GeV]");
				
	}
	for (auto h1 : hList)
	{
	for(int j=0;j<N;j++)
	{
		partFits[j]=h1->GetFunction(partNames[j]);	//stored fits		
	}
	for(int i=0;i<mbin+1;i++) //iterate by number of bins 
	{
		sMax=0;
		
		for(int j=0;j<N;j++) //iterate for every particle
		{
			sVal[j]=partFits[j]->Eval(mmin+(i*mbinsize));
			
			//cout<< partFits[j]->Eval(mmin+(i*mbinsize))<<'\n';
			//cout<<j<<endl;
		}
		sSum=0;
		
		/*
		if(sVal[5]<0)
		{
			cout<<h1->GetName()<<endl; //black box intensifies as bacground signal is negative
		}
		*/
		
		for(int j=0;j<5;j++) //iterate for every real particle !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
		{
			sSum=sSum+sVal[j];
		}
	
		for(int j=0;j<5;j++) //iterate for every real particle !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
		{
			partHist[j]->SetBinContent(i,k,sVal[j]/sVal[6]);
			if(sVal[j]>sMax)
			{
				sMax=sVal[j];
			}
			//cout<< partFits[j]->Eval(mmin+(i*mbinsize))<<'\n';
			//cout<<j<<endl;
		}
		partHist[6]->SetBinContent(i,k,sMax/sVal[6]);
		
		
		
		
		
		sMax=0;
		for(int j=0;j<5;j++) //iterate for every real particle !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
		{
			if(sVal[j]/sVal[6]>1)
			{
				sVal[j]=sVal[6];
			}
			partHistReal[j]->SetBinContent(i,k,sVal[j]/sVal[6]);
			if(sVal[j]>sMax)
			{
				sMax=sVal[j];
			}
			//cout<< partFits[j]->Eval(mmin+(i*mbinsize))<<'\n';
			//cout<<j<<endl;
		}
		
		
		partHistReal[6]->SetBinContent(i,k,sMax/sVal[6]);
	
	}
	k++;
	}

}
