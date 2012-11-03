
// GEANT4 Includes
#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>

#include "ComptonG4PrimaryGeneratorAction.hh"
#include "ComptonG4PrimaryGeneratorMessenger.hh"
#include "ComptonG4Analysis.hh"

ComptonG4PrimaryGeneratorAction::ComptonG4PrimaryGeneratorAction(ComptonG4Analysis *analysis) :

  fAnalysis(analysis)
{
  G4int n_particle = 2; // Gun shoots photons ( and may shoot electrons too)
  fParticleGun = new G4ParticleGun(n_particle);

  // Messenger class
  fGunMessenger = new ComptonG4PrimaryGeneratorMessenger(this);

  // Default mode is a standard GEANT4 gun
  fGeneratorMode = 0;

  // Initialize all settings to zero (no hard coded values allowed here!!)
  SetPhotonVertex(G4ThreeVector(0.0,0.0,0.0));
  SetPhotonDivergence(0.0);
  SetPhotonTrackStart(0.0);
  SetElectronEnergy(0.0);
  SetLaserWavelength(0.0);
  SetMaxPhotonEnergy(0.0);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String gammaName;
  G4String electronName;
  fGammaDef = particleTable->FindParticle(gammaName="gamma");
  fElectronDef = particleTable->FindParticle(electronName="electron");
}
