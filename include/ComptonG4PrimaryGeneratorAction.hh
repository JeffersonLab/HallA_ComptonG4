
#ifndef COMPTONG4PRIMARYGENERATORACTION_HH
#define COMPTONG4PRIMARYGENERATORACTION_HH

// Geant4 Includes
#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ThreeVector.hh>

// Pre-defined classes
class ComptonG4Analysis;
class G4Event;
class G4ParticleGun;
class ComptonG4PrimaryGeneratorMessenger;
class G4ParticleDefinition;

class ComptonG4PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  ComptonG4PrimaryGeneratorAction(ComptonG4Analysis *analysis = 0);
  ~ComptonG4PrimaryGeneratorAction() {}

  void GeneratePrimaries(G4Event *event) {}

  // Setup setters
  void SetElectronEnergy(G4double val) { fElectronEnergy = val; }
  void SetMaxPhotonEnergy(G4double val) { fMaxPhotonEnergy = val; }
  void SetLaserWavelength(G4double val) { fLaserWavelength = val; }
  void SetGeneratorMode(G4int val){fGeneratorMode=val;}
  void SetPhotonX(G4double val) { fPrimaryVertexLocation.setX(val); }
  void SetPhotonY(G4double val) { fPrimaryVertexLocation.setY(val); }
  void SetPhotonZ(G4double val) { fPrimaryVertexLocation.setZ(val); }
  void SetPhotonVertex(G4ThreeVector vec) { fPrimaryVertexLocation = vec; }
  void SetPhotonDivergence(G4double val) { fPhotonDivergence = val; }
  void SetPhotonTrackStart(G4double val) { fPhotonTrackStart = val; }

private:
  ComptonG4Analysis *fAnalysis; // Our IO class
  G4ParticleGun *fParticleGun;
  ComptonG4PrimaryGeneratorMessenger *fGunMessenger;

  // Settings
  G4ThreeVector fPrimaryVertexLocation; // Primary generation point for photons
  G4double fElectronEnergy;       // Electron beam energy
  G4double fLaserWavelength;      // Laser wavelength
  G4double fMaxPhotonEnergy;      // Photon energy cutoff
  G4double fPhotonDivergence;     // Photon divergence
  G4double fPhotonTrackStart;     // Length he photon is tracked
  G4int fGeneratorMode;           // Gun generator mode

  // Hold the particle definitions
  G4ParticleDefinition* fGammaDef;    // Gamma definition pointer
  G4ParticleDefinition* fElectronDef; // Electron definition pointer
};

#endif
