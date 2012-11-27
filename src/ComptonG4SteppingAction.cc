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
  if (track->GetDefinition()->GetParticleName() == "gamma" &&
      !strcmp(volume->GetName(),"LeadTungstateCrystalVolume_PV")) {
    fAnalysis->AddEDep(step->GetTotalEnergyDeposit()/MeV);
  }
}
