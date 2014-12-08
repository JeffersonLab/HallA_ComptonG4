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

ComptonG4SteppingAction::ComptonG4SteppingAction(ComptonG4Analysis *analysis) :
fAnalysis(analysis),fOpticalMaxStepTime(1.0e-6*CLHEP::s),fVerbose(0)
{
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

  if( track->GetDefinition() == G4OpticalPhoton::Definition() ) {
    if ( track->GetGlobalTime() > fOpticalMaxStepTime ) {
      track->SetTrackStatus(fStopAndKill);
      fAnalysis->StoppedOpticalPhoton();
    }
  }
}
