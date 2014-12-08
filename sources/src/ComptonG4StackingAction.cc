#include "ComptonG4StackingAction.hh"

// GEANT4 Includes
#include <G4Track.hh>
#include <G4OpticalPhoton.hh>

/*
 *
 */
ComptonG4StackingAction::ComptonG4StackingAction() :
  fOpticalPhotonMinEnergy(0*CLHEP::eV),
  fOpticalPhotonMaxEnergy(0*CLHEP::eV)
{
  // TODO: Don't hard code this in! It should be set by a messenger
  fOpticalPhotonMinEnergy = 0.0*CLHEP::eV;
  fOpticalPhotonMaxEnergy = 10.0*CLHEP::eV;
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
    if(track->GetKineticEnergy() < fOpticalPhotonMinEnergy) {
      // Too small an energy to care, kill the particle now
      G4cout << "Energy too low ( " << track->GetKineticEnergy()/CLHEP::eV
        << " eV): stopping!" << G4endl;
      return fKill;
    }

    if(track->GetKineticEnergy() > fOpticalPhotonMaxEnergy) {
      // Too large an energy to care, kill the particle now
      G4cout << "Energy too high ( " << track->GetKineticEnergy()/CLHEP::eV
        << " eV): stopping!" << G4endl;
      return fKill;
    }

  }

  // Do the default GEANT4 thing
  return G4UserStackingAction::ClassifyNewTrack(track);
}
