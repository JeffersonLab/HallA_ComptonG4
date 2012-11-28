///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// Local Includes
#include "ComptonG4SteppingAction.hh"
#include "ComptonG4Analysis.hh"

// GEANT4 Includes
#include <G4VPhysicalVolume.hh>
#include <G4Track.hh>

ComptonG4SteppingAction::ComptonG4SteppingAction(ComptonG4Analysis *analysis) :
  fAnalysis(analysis)
{

}

ComptonG4SteppingAction::~ComptonG4SteppingAction()
{
}

void ComptonG4SteppingAction::UserSteppingAction(const G4Step* step)
{
  const G4Track* track = step->GetTrack();
  G4VPhysicalVolume* volume = track->GetVolume();
  G4ThreeVector pos(track->GetPosition());
  // TODO: The detector should not be hard coded!! Use the Active Detectors
  // command to specify which detector to use
  if (track->GetDefinition()->GetParticleName() == "gamma" &&
      !strcmp(volume->GetName(),"LeadTungstateCrystal_Logical_PV")) {
    fAnalysis->AddEDep(step->GetTotalEnergyDeposit()/MeV);
  }
}
