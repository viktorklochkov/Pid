void RunGetter(TString InputFile="../build/pid_getter.root")
{

    std::unique_ptr <TFile> f2{TFile::Open(InputFile)};        
    std::unique_ptr <Pid::Getter> getter {(Pid::Getter*) f2->Get("pid_getter")}; 
    
    std::unique_ptr<TRandom> r{new TRandom};
    
    for (int i=0; i<10; ++i)
    {
        const float m2 = r->Uniform(-0.1, 1);
        const float p = r->Uniform(1, 5);
        
        auto prob = getter->GetBayesianProbability(p, m2);
        
        std::cout << "m2 = " << m2 << " p = " << p << " proton probability = " << prob[PidParticles::kProton] << std::endl;
        std::cout << "m2 = " << m2 << " p = " << p << " kaon   probability = " << prob[PidParticles::kKaon] << std::endl;
        std::cout << "m2 = " << m2 << " p = " << p << " pion   probability = " << prob[PidParticles::kPion] << std::endl;
        std::cout << "m2 = " << m2 << " p = " << p << " bg     probability = " << prob[PidParticles::kBg] << std::endl;
    }
    
    f2->Close();
}
