void plot()
{
  TChain *chain = new TChain("ComptonG4");
  chain->Add("ComptonG4_background_study00*.root");
  Double_t entries = chain->GetEntries();

  std::cout << "Total entries: " << entries << std::endl;
  chain->SetAlias("total_eDep","(Crystal1_eDep+Crystal2_eDep+Crystal3_eDep+Crystal4_eDep)");
  //chain->Draw("total_eDep>>hist","total_eDep>0&&total_eDep<100");
  chain->Draw("total_eDep>>hist","total_eDep>0&&total_eDep<1e20");
  gPad->SetLogy(kTRUE);
  TH1F *hist = (TH1F*)gDirectory->Get("hist");
  //hist->Scale(1./200000.);
  /*
  Double_t current = 180e-6;
  Double_t charge = 1.60217657e-19;
  Double_t numelectrons = chain->GetEntries();
  Double_t rate=(current/charge)/numelectrons;
  Double_t torrToAtm = 1./760.;
  Double_t helicityWindowTime = 1.0/960.0;
  */
  Double_t eCharge = 1.60217657e-19;
  Double_t beamCurrent = 180e-6;
  Double_t numEPerSecInBeam = beamCurrent/eCharge; // # Electrons per second
  Double_t numEInSimulation = entries;
  Double_t ratio = numEPerSecInBeam/numEInSimulation;
  Double_t pressure = 760.0; // torr
  Double_t vaccum = 1e-6.0;
  Double_t densityRatio = vaccum/pressure;
  hist->SetTitle("Energy Deposited;Energy MeV;Number of Particles (Hz)");
  //hist->Scale(rate*torrToAtm*1e-6);
  hist->Scale(densityRatio*ratio*1e-3);
  hist->Draw();
  Double_t integral = hist->Integral();
  //std::cout << "Total number of particles: " << integral << std::endl;
  std::cout << "Total Number of Particles for Vaccum Pressure" << std::endl;
  for(Int_t i = 1; i < 11; i++ ) {
    std::cout << i << "uTorr  \t" << integral*Double_t(i) << std::endl;
  }
}
