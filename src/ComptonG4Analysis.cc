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
  fRho(0.0),fGammaE(0.0),fEDep(0.0),fStepSize(0.0),fNumberOfEvents(0)
{
  char *out = getenv("CG4_OUTPUT_PATH");
  if(out!=NULL) {
    fOutputPath = G4String(out)+"/";
  } else {
    fOutputPath = "";
  }
}

/**
 *
 */
ComptonG4Analysis::~ComptonG4Analysis()
{
}

/**
 * Initialize the Analyzer with the specified run number
 * @param runnum
 */
void ComptonG4Analysis::Initialize( G4int runnum )
{
  G4String outFile = fOutputPath + "ComptonG4_"+
      G4UIcommand::ConvertToString(runnum)+".root";
  fFile = new TFile(outFile,"RECREATE");
  fTree = new TTree("ComptonG4","Compton GEANT4 Simulation Results");
  fTree->Branch("asym",&fAsym,"asym/D");
  fTree->Branch("rho",&fRho,"rho/D");
  fTree->Branch("gammaE",&fGammaE,"gammaE/D");
  fTree->Branch("eDep",&fEDep,"eDep/D");
  fTree->Branch("stepSize",&fStepSize,"stepSize/D");
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
  fAsym = fRho = fGammaE = fEDep = fStepSize = 0.0;
}

/**
 *
 */
void ComptonG4Analysis::Finished()
{
  fFile->Write();
}
