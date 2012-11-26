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
ComptonG4Analysis::ComptonG4Analysis() : fTree(0),fFile(0)
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
  fTree->Branch("asym/D",&fAsym);
  fTree->Branch("rho/D",&fRho);
  fTree->Branch("gammaE/D",&fGammaE);
  fTree->Branch("eDep/D",&fEDep);
}

/**
 *
 */
void ComptonG4Analysis::StartOfEvent()
{
  // Reset all values!
  fAsym = fRho = fGammaE = fEDep = 0.0;
}

/**
 *
 */
void ComptonG4Analysis::EndOfEvent()
{
  // Fill the tree
  fTree->Fill();
}

/**
 *
 */
void ComptonG4Analysis::FinishRun()
{
  fFile->Close();
}
