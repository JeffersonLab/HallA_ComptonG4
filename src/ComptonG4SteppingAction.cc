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
/*  const G4Track* track = step->GetTrack();
  G4VPhysicalVolume* volume = track->GetVolume();
  G4ThreeVector pos(track->GetPosition());
//  if (track->GetDefinition()->GetParticleName() == "gamma" ) {
    G4cout << "Gamma pos: (" << pos.getX() << " " << pos.getY() << " "
        << pos.getZ() << ") \t E: " << track->GetKineticEnergy() << "\t P=: ("
        << track->GetMomentumDirection().getX() << ","
        << track->GetMomentumDirection().getY()
        << "," << track->GetMomentumDirection().getY() << ")"
        << " \tStep#: " << track->GetCurrentStepNumber()
        << " \tLength: " << step->GetStepLength()
        << " \tTime: " << step->GetDeltaTime()
        << " \tName: " << track->GetDefinition()->GetParticleName()
        << "\n";
  } */
}
