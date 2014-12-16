///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// Local Includes
#include "ComptonG4SteppingAction.hh"
#include "ComptonG4SteppingMessenger.hh"
#include "ComptonG4Analysis.hh"

// GEANT4 Includes
#include <G4VPhysicalVolume.hh>
#include <G4Track.hh>
#include <G4VProcess.hh>
#include <G4OpticalPhoton.hh>

// ROOT includes
#include <TTree.h>

ComptonG4SteppingAction::ComptonG4SteppingAction(ComptonG4Analysis *analysis) :
fAnalysis(analysis),fOpticalMaxStepTime(1.0e-6*CLHEP::s),fVerbose(0)
{
  // Initialize the stored data pointer
  fPrimaryDataPtr = new std::vector<ComptonG4PrimaryData>;

  // Tell the analyzer about us
  analysis->SetSteppingAction(this);

  // Messenger class
  fStepMessenger = new ComptonG4SteppingMessenger(this);
}

ComptonG4SteppingAction::~ComptonG4SteppingAction()
{
}

void ComptonG4SteppingAction::UserSteppingAction(const G4Step* step)
{

  G4Track *track = step->GetTrack();
  if( !track )
    return;

  // We want to track the primary particle all the way through
  if( track->GetParentID() == 0 ) {
    ComptonG4PrimaryHit hit;
    hit.ProcessTrack(track);
    hit.SetEnergyDeposited(step->GetTotalEnergyDeposit());
    fPrimaryHits.push_back(hit);
    fPrimaryDataPtr->push_back(hit.GetData());
  }

  if( track->GetDefinition() == G4OpticalPhoton::Definition() ) {
    if ( track->GetGlobalTime() > fOpticalMaxStepTime ) {
      ComptonG4Hit hit;
      hit.ProcessTrack(track);
      fStoppedOpticalHits.push_back(hit);
      fStoppedOpticalDataPtr->push_back(hit.GetData());
      track->SetTrackStatus(fStopAndKill);
      fAnalysis->StoppedOpticalPhoton();
    }
  }
}

/*
 *
 */
void ComptonG4SteppingAction::CreateTreeBranch(TTree* tree)
{
  // Create detector branches
  tree->Branch("Primary",&(fPrimaryDataPtr));
  tree->Branch("OpticalPhotons_stopped",&(fStoppedOpticalDataPtr));
}


void ComptonG4SteppingAction::ClearPrimary()
{
  fPrimaryHits.clear();
  fPrimaryDataPtr->clear();
  fStoppedOpticalHits.clear();
  fStoppedOpticalDataPtr->clear();
}
