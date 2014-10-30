#include "ComptonG4StackingAction.hh"

// GEANT4 Includes
#include <G4Track.hh>
#include <G4OpticalPhoton.hh>

/*
 *
 */
ComptonG4StackingAction::ComptonG4StackingAction() :
  fOpticalPhotonCutoffEnergy(0*CLHEP::eV)
{
  // TODO: Don't hard code this in! It should be set by a messenger
  fOpticalPhotonCutoffEnergy = 1.7*CLHEP::eV;
}

/*
 *
 */
ComptonG4StackingAction::~ComptonG4StackingAction()
{

}

/*
 *
 */
G4ClassificationOfNewTrack ComptonG4StackingAction::ClassifyNewTrack(
    const G4Track* track)
{
  if( track->GetDefinition() == G4OpticalPhoton::Definition() ) {
    if(track->GetKineticEnergy() < fOpticalPhotonCutoffEnergy) {
      // Too small an energy to care, kill the particle now
      return fKill;
    }
  }

  // Do the default GEANT4 thing
  return G4UserStackingAction::ClassifyNewTrack(track);
}
