/*
  to run go into build directory and run something like:
  ./cg4root ../analysis/drawSomething.C\(\"./test00043.root\"\)
*/
void drawSomething(string fnm){

  TFile *fin=TFile::Open(fnm.c_str(),"READ");
  TTree *t=(TTree*)fin->Get("ComptonG4");
  double asym, rho, gammaE;
  std::vector<ComptonG4PrimaryData> *event=0;
  t->SetBranchAddress("asym",&asym);
  t->SetBranchAddress("rho",&rho);
  t->SetBranchAddress("gammaE",&gammaE);
  t->SetBranchAddress("Primary",&event);

  int nEv= t->GetEntries();
  cout<<"evts "<<nEv<<endl;

  TH1D *hZ = new TH1D ("hZ","hZ",100,-10000,10000);

  for(int i=0;i<nEv;i++){
    t->GetEntry(i);

    unsigned int nHits = event->size();
    for(int j=0;j<nHits;j++){
      if(event->at(j).volume=="ComptonVacuumDetector1")
	hZ->Fill(event->at(j).position[2]);
    }

  }
  hZ->DrawCopy();
  fin->Close();
  delete fin;
}
