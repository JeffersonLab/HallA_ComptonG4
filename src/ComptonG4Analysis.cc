/*!
 *
 * @author Juan Carlos Cornejo
 !*/
//
//
////////////////////////////////////////////////////////////////////////////////

// Local includes
#include "ComptonG4Analysis.hh"

// ROOT Includes
#include <TFile.h>
#include <TTree.h>

// system includes
#include <sstream>
#include <fstream>

/**
 *
 */
ComptonG4Analysis::ComptonG4Analysis() : fTree(0),fFile(0),fAsym(0.0),
  fRho(0.0),fGammaE(0.0),fTheta(0.0),fPhi(0.0),
  fNumberOfOpticalPhotonsStopped(0.0),fNumberOfOpticalPhotonsProduced(0.0),
  fTotalNumberOfOpticalPhotonsAbsorbed(0.0),fNumberOfEvents(0),
  fAutoSaveEntry(0)
{
  // Empty out vectors and stuff
  CleanEvent();

  fOutputPath = "";
}

void ComptonG4Analysis::SetOutputPath( std::string path )
{
  fOutputPath = path+"/";
}

/**
 *
 */
ComptonG4Analysis::~ComptonG4Analysis()
{
  fDetectors.clear();
  fDetectorNames.clear();
}

/**
 *
 */
void ComptonG4Analysis::AddDetector(std::string name)
{
  DetectorStruct det;
  std::vector<Double_t> time;
  det.eDep = 0.0;
  det.nOpticalPhotons = 0.0;
  if(DetectorIndex(name) < 0 ) {
    fDetectorNames.push_back(name);
    fDetectors.push_back(det);
    fDetectorTimes.push_back(time);
  } else {
    std::cout << "Already have " << name << " on the list!" << std::endl;
  }
}

/**
 *
 */
void ComptonG4Analysis::SetEDep(std::string name, Double_t e)
{
  G4int index = DetectorIndex(name);
  if(index<0)
    return;

  fDetectors[index].eDep = e;
}

void ComptonG4Analysis::SetOpticalHits(std::string name, Double_t hits)
{
  G4int index = DetectorIndex(name);
  if(index<0)
    return;

  fDetectors[index].nOpticalPhotons = hits;
}

void ComptonG4Analysis::SetGlobalTimes(std::string name,
    std::vector<Double_t> times)
{
  G4int index = DetectorIndex(name);
  if(index<0)
    return;

  for(unsigned int i = 0; i < times.size(); i++ ) {
    fDetectorTimes[index].push_back(times[i]);
  }
}



/**
 *
 */
int ComptonG4Analysis::DetectorIndex(std::string name)
{
  for(unsigned int i = 0; i < fDetectorNames.size(); i++ ) {
    if(fDetectorNames[i].compare(name)==0) {
      return i;
    }
  }

  // Not found
  return -1;
}
/**
 * Initialize the Analyzer with the specified run number
 * @param runnum
 */
void ComptonG4Analysis::Initialize( G4int runnum, unsigned int auto_save)
{
  fRunNumber = runnum;
  fAutoSaveEntry = auto_save;
  std::string outFile = fOutputPath + "ComptonG4_"+
      G4UIcommand::ConvertToString(runnum)+".root";
  fFile = new TFile(outFile.c_str(),"RECREATE");
  fTree = new TTree("ComptonG4","Compton GEANT4 Simulation Results");
  fTree->Branch("asym",&fAsym,"asym/D");
  fTree->Branch("rho",&fRho,"rho/D");
  fTree->Branch("gammaE",&fGammaE,"gammaE/D");
  fTree->Branch("theta",&fTheta,"theta/D");
  fTree->Branch("phi",&fPhi,"phi/D");
  fTree->Branch("nPhotonsStopped",&fNumberOfOpticalPhotonsStopped,"nPhotonsStopped/D");
  fTree->Branch("nPhotonsProduced",&fNumberOfOpticalPhotonsProduced,"nPhotonsProduced/D");
  fTree->Branch("nTotalPhotonsAbsorbed",&fTotalNumberOfOpticalPhotonsAbsorbed,"nTotalPhotonsAbsorbed/D");

  // Create detector branches
  for(size_t i = 0; i < fDetectorNames.size(); i++ ) {
    fTree->Branch(fDetectorNames[i].c_str(),&(fDetectors[i].eDep),
        "eDep/D:nOpticalPhotons/D");

    fTree->Branch(Form("%s%s",fDetectorNames[i].c_str(),"Times"),
          &(fDetectorTimes[i]));
  }

  CleanEvent();
  fNumberOfEvents = 0;
}

/**
 *
 */
void ComptonG4Analysis::StartOfEvent()
{
}

/**
 *
 */
void ComptonG4Analysis::EndOfEvent()
{
  // Fill the tree
  fTree->Fill();

  // AutoSave?
  if(fAutoSaveEntry&& fNumberOfEvents%fAutoSaveEntry == 0) {
    G4cout << "Auto saving..." << G4endl;
    fTree->AutoSave("SaveSelf");
    WriteRandomSeed();
  }
  fNumberOfEvents++;
  if ( ( fNumberOfEvents % 10000 ) == 0) {
    G4cout << "Processed event: " << fNumberOfEvents << G4endl;
  }

  // Clean the events (otherwise those vectors quickly add up!!
  CleanEvent();
}

void ComptonG4Analysis::CleanEvent()
{
  // Reset all values!
  fAsym = fRho = fGammaE = fTheta = fPhi = fNumberOfOpticalPhotonsStopped
    = fNumberOfOpticalPhotonsProduced = fTotalNumberOfOpticalPhotonsAbsorbed
    = 0.0;
  for(size_t i = 0; i < fDetectors.size(); i++ ) {
    fDetectors[i].eDep = 0.0;
    fDetectors[i].nOpticalPhotons = 0.0;
    fDetectorTimes[i].clear();
  }
  fOpticalTrackIDs.clear();
}

/**
 *
 */
void ComptonG4Analysis::Finished()
{
  fFile->Write();
  WriteRandomSeed();
}



void ComptonG4Analysis::StoreRandomSeed(G4String seed)
{
  fRandomSeed = seed;
}

/*
 *
 */
void ComptonG4Analysis::WriteRandomSeed()
{
  // Now save the random seed
  std::stringstream name;
  name << fOutputPath << "/run_" << fRunNumber
    << "_event_" << fNumberOfEvents << ".rndm";
  std::ofstream file(name.str().c_str()); // why can't fstream take a string?
  TString seed(fRandomSeed); // copy
  seed.Remove(0,TString(fRandomSeed).First('\n') + 1); // remove first line with garbage
  file << seed; // write to file
}

/*
 *
 */
void ComptonG4Analysis::ProcessOpticalTrackID(G4int id)
{
  Bool_t found = false;
  for(UInt_t i = 0; i < fOpticalTrackIDs.size() && !found; i++ ) {
    if(id==fOpticalTrackIDs[i]) {
      found = true;
    }
  }

  if(!found) {
    fNumberOfOpticalPhotonsProduced += 1.0;
    fOpticalTrackIDs.push_back(id);
  }

}
