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

/**
 *
 */
ComptonG4Analysis::ComptonG4Analysis() : fTree(0),fFile(0),fAsym(0.0),
  fRho(0.0),fGammaE(0.0),fTheta(0.0),fPhi(0.0),fNumberOfEvents(0)
{
  char *out = getenv("CG4_OUTPUT_PATH");
  if(out!=NULL) {
    fOutputPath = G4String(out)+"/";
  } else {
    fOutputPath = "";
  }

  // Empty out vectors and stuff
  CleanEvent();
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
  det.eDep = 0.0;
  det.nOpticalPhotons = 0.0;
  if(DetectorIndex(name) < 0 ) {
    fDetectorNames.push_back(name);
    fDetectors.push_back(det);
  }
}

/**
 *
 */
void ComptonG4Analysis::AddEDep(std::string name, Double_t e)
{
  G4int index = DetectorIndex(name);
  if(index<0)
    return;

  fDetectors[index].eDep += e;
}

void ComptonG4Analysis::AddOpticalHits(std::string name, Double_t hits)
{
  G4int index = DetectorIndex(name);
  if(index<0)
    return;

  fDetectors[index].nOpticalPhotons += hits;
}


/**
 *
 */
int ComptonG4Analysis::DetectorIndex(std::string name)
{
  for(int i = 0; i < fDetectorNames.size(); i++ ) {
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
void ComptonG4Analysis::Initialize( G4int runnum )
{
  std::string outFile = fOutputPath + "ComptonG4_"+
      G4UIcommand::ConvertToString(runnum)+".root";
  fFile = new TFile(outFile.c_str(),"RECREATE");
  fTree = new TTree("ComptonG4","Compton GEANT4 Simulation Results");
  fTree->Branch("asym",&fAsym,"asym/D");
  fTree->Branch("rho",&fRho,"rho/D");
  fTree->Branch("gammaE",&fGammaE,"gammaE/D");
//  fTree->Branch("eDep",&fEDep,"eDep/D");
//  fTree->Branch("stepSize",&fStepSize,"stepSize/D");
//  fTree->Branch("eDepAllCharges",&fEDepAll,"eDepAllCharges/D");
//  fTree->Branch("stepSizeAllCharges",&fStepSizeAll,"stepSizeAllCharges/D");
  fTree->Branch("theta",&fTheta,"theta/D");
  fTree->Branch("phi",&fPhi,"phi/D");

  // Create detector branches
  for(size_t i = 0; i < fDetectorNames.size(); i++ ) {
    fTree->Branch(fDetectorNames[i].c_str(),&(fDetectors[i]),
        "eDep/D:nOpticalPhotonsWhat/D");
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
  //if(fEDep > 0. ) {
    // Fill the tree
    fTree->Fill();
  //}
  CleanEvent();
  fNumberOfEvents++;
  if ( ( fNumberOfEvents % 10000 ) == 0) {
    G4cout << "Processed event: " << fNumberOfEvents << G4endl;
  }
}

void ComptonG4Analysis::CleanEvent()
{
  // Reset all values!
  fAsym = fRho = fGammaE = fTheta;
  for(size_t i = 0; i < fDetectors.size(); i++ ) {
    fDetectors[i].eDep = 0.0;
    fDetectors[i].nOpticalPhotons = 0.0;
  }
}

/**
 *
 */
void ComptonG4Analysis::Finished()
{
  fFile->Write();
}


