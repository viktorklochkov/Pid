void RunGetter(TString InputFile="../build/pid_getter.root")
{

    std::unique_ptr <TFile> f2{TFile::Open(InputFile)};        
    std::unique_ptr <Pid::Getter> getter {(Pid::Getter*) f2->Get("pid_getter")}; 
    
    const float m2 = 0.1;
    const float p = 0.5;
    
    auto prob = getter->GetBayesianProbability(p, m2);
    
    std::cout << "proton probability = " << prob[PidParticles::kProton] << std::endl;
    std::cout << "kaon   probability = " << prob[PidParticles::kKaon] << std::endl;
    std::cout << "pion   probability = " << prob[PidParticles::kPion] << std::endl;
    std::cout << "bg     probability = " << prob[PidParticles::kBg] << std::endl;
    
    f2->Close();
}
