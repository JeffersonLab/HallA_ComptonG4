// ComptonG4 Includes:
#include "ComptonG4PrimaryGeneratorAction.hh"
#include "ComptonG4PrimaryGeneratorMessenger.hh"
#include "ComptonG4Analysis.hh"

// GEANT4 Includes:
#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <Randomize.hh>
#include <G4OpticalPhoton.hh>

// Standard Includes:
#include <cmath>

ComptonG4PrimaryGeneratorAction::ComptonG4PrimaryGeneratorAction(ComptonG4Analysis *analysis) :

  fAnalysis(analysis), fTransversePol(0.0), fLongitudinalPol(1.0), fVerbose(0)
{
  G4int n_particle = 1; // Gun shoots photons (and may shoot electrons too)
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
  fGammaDef = particleTable->FindParticle("gamma");
  fElectronDef = particleTable->FindParticle("e-");
  fOpticalDef = particleTable->FindParticle("optical");

  // Set a default primary Z vertex
  fPrimaryVertexLocation = G4ThreeVector(0*CLHEP::mm,0*CLHEP::mm,0*CLHEP::mm);
}

ComptonG4PrimaryGeneratorAction::~ComptonG4PrimaryGeneratorAction()
{
  if(fParticleGun)
    delete fParticleGun;

  if(fGunMessenger)
    delete fGunMessenger;
}


void ComptonG4PrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
  // We generate the primaries based on the selected mode
  switch(fGeneratorMode) {
  case 1:
    GeneratePrimaryMonoEnergeticMode();
    break;
  case 2:
    break;
  case 3: // Compton mode (i.e, real physics mode)
    GeneratePrimaryComptonMode(); // Generates both Compton photon, electron eventually. Currently just photons 
    break;
  case 4: // Optical photon mode
    GeneratePrimaryOpticalMode();
    break;
  case 5: // Polarized Electrons mode
    GeneratePrimaryPolarizedElectronsMode();
    break;
  case 6: // GenerateComptonElectronMode() **do this first! 2016-06-29
    GenerateComptonElectronMode();
    break;
    //case 7: // GenerateComptonPhotonMode() // Just Compton photons
  default: // Do nothing, use the default GEANT4 gun
    break;
  };
  fParticleGun->GeneratePrimaryVertex(event);
}


void ComptonG4PrimaryGeneratorAction::Initialize()
{
  // Initialize this generator. If we are using Compton mode, one of the
  // first things to initialize is the distribution function (cross section)

  if ( fGeneratorMode == 3 ) {
    fLaserEnergy = CLHEP::h_Planck * CLHEP::c_light / fLaserWavelength;
    fAParameter = 1 / (1 + (4*fLaserEnergy*fElectronEnergy)/
        (CLHEP::electron_mass_c2*CLHEP::electron_mass_c2));
    fMaxPhotonEnergy = fElectronEnergy * (1 - fAParameter);
    // Compton scattering cross section (eqn 4.8):
    G4double am1 = fAParameter - 1.0;
    for(int i = 0; i < 10000; i++) {
      G4double rho = G4double(i/10000.0);
      G4double term1 = rho*rho*am1*am1/(1.+rho*am1);
      G4double term3 = (1.-rho*(1.0+fAParameter))/(1.+rho*am1);
      fCXdSig_dRho[i] = 2 * CLHEP::pi * CLHEP::classic_electr_radius*
        CLHEP::classic_electr_radius * fAParameter *(term1 +1.0 + term3*term3);
    }
  }

  G4cout << "Max Scattered Photon Energy: "
    << fMaxPhotonEnergy/CLHEP::MeV << " MeV" << G4endl;
}

G4double ComptonG4PrimaryGeneratorAction::GetRandomRho()
{
  G4RandGeneral GenDist(fCXdSig_dRho,10000);
  return GenDist.shoot();
}

void ComptonG4PrimaryGeneratorAction::GeneratePrimaryMonoEnergeticMode()
{
  fParticleGun->SetParticleEnergy(fMaxPhotonEnergy);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
  fParticleGun->SetParticleDefinition(fGammaDef);
  fAnalysis->SetAsym(0.0);
  fAnalysis->SetRho(0.0);
  fAnalysis->SetGammaE(fMaxPhotonEnergy/CLHEP::MeV);
  fAnalysis->SetTheta(0.0);
  fAnalysis->SetPhi(0.0);
}

void ComptonG4PrimaryGeneratorAction::GeneratePrimaryOpticalMode()
{
  G4ThreeVector opticalDirection = G4ThreeVector(0.0,0.0,1.0);
  opticalDirection.setRThetaPhi(1.0,
      CLHEP::RandFlat::shoot(CLHEP::pi/4.0)/CLHEP::radian,
      CLHEP::RandFlat::shoot(2.0*CLHEP::pi)/CLHEP::radian);
  fParticleGun->SetParticleEnergy(
      CLHEP::RandFlat::shoot(1.0) * fMaxPhotonEnergy);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(opticalDirection);
  fParticleGun->SetParticlePolarization(opticalDirection);
  fParticleGun->SetParticleDefinition(G4OpticalPhoton::Definition());
}

void ComptonG4PrimaryGeneratorAction::GeneratePrimaryComptonMode()
{
  G4double gammaE = 0.0;      // Scattered photon energy
  G4double rho = 0.0;         // Normalized photon energy gammaE/ComptonEdge
  G4double gammaTheta = 0.0;  // Photon scattered polar angle
  G4double gammaPhi = 0.0;    // Photon scattered azimuthal angle
  G4ThreeVector gammaDirection(0.0, 0.0, 1.0); // This will hold the direction
              // of the scattered photon. 
              // For now, we just point it towards the photon detector.

  rho = GetRandomRho();
  gammaE = rho * fMaxPhotonEnergy;
  // Theta from Megan Friend's CompCal code:
  G4double tmp = CLHEP::electron_mass_c2 / fElectronEnergy;
  gammaTheta = std::sqrt( 4 * fLaserEnergy * fAParameter / gammaE -(tmp*tmp));
  gammaPhi = CLHEP::RandFlat::shoot(2.0 * CLHEP::pi); // Arbitrary
  gammaDirection.setRThetaPhi(1.0, gammaTheta/CLHEP::radian,
      gammaPhi / CLHEP::radian);

  fParticleGun->SetParticleEnergy(gammaE);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(gammaDirection);
  fParticleGun->SetParticleDefinition(fGammaDef);
  /*
  if (fVerbose > 0)
    G4cout << "Direction: (" << gammaDirection.getX()/mm << ","
      << gammaDirection.getY()/mm << "," << gammaDirection.getZ()/mm << ")\n";
  */
  fAnalysis->SetAsym(GetComptonAsym(rho));
  fAnalysis->SetRho(rho);
  fAnalysis->SetGammaE(gammaE/CLHEP::MeV);
  fAnalysis->SetTheta(57.2957795 * gammaTheta / CLHEP::radian);
  fAnalysis->SetPhi(57.2957795 * gammaPhi / CLHEP::radian);
  if (fVerbose > 0)
    G4cout << "Shooting photon with energy: " << gammaE/CLHEP::MeV << " MeV"
      << G4endl;
}

// Added case 6: 2016-06-29 (L Thorne)
void ComptonG4PrimaryGeneratorAction::GenerateComptonElectronMode(){

  // Use conservation of E, p; knowing photon stuff:
  // Do all photon stuff again: ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  G4double gammaE = 0.0;      // Scattered photon energy
  G4double rho = 0.0;         // Normalized photon energy gammaE/ComptonEdge
  G4double gammaTheta = 0.0;  // Photon scattered polar angle
  G4double gammaPhi = 0.0;    // Photon scattered azimuthal angle
  G4ThreeVector gammaDirection(0.0, 0.0, 1.0); // This will hold the direction
              // of the scattered photon. 
              // For now, we just point it towards the photon detector.
  rho = GetRandomRho();
  gammaE = rho * fMaxPhotonEnergy;
  // Theta from Megan Friend's CompCal code:
  G4double tmp = CLHEP::electron_mass_c2 / fElectronEnergy;
  gammaTheta = std::sqrt( 4 * fLaserEnergy * fAParameter / gammaE -(tmp*tmp));
  gammaPhi = CLHEP::RandFlat::shoot(2.0 * CLHEP::pi); // Arbitrary
  gammaDirection.setRThetaPhi(1.0, gammaTheta/CLHEP::radian,
      gammaPhi / CLHEP::radian);
  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  G4double electronE = 0.0;    // Scattered electron energy
  G4double electronTheta = 0.0; // Scattered electron polar angle
  G4ThreeVector electronDirection(0.0, 0.0, 1.0); // Into e- detector?
  G4double electronP = 0.0;    // Initial electron momentum (z dir) 
  G4double electronMinEnergy = 0.0; // Corresponds to Compton edge
  G4double electronRho = 0.0;  // Normalized e- energy: electronE/ComptonEdge
  G4double electronPhi = 0.0; // Azimuthal scattered electron dir

  electronE = (fElectronEnergy + fLaserEnergy) - gammaE; // E cons
  electronP = std::sqrt(std::pow(fElectronEnergy, 2) - std::pow(CLHEP::electron_mass_c2, 2));	
  electronTheta = std::asin(gammaE * std::sin(gammaTheta) / electronP);//Px cons
  electronMinEnergy = fElectronEnergy - gammaE; // Megan: Eqn 4.4
  electronRho = electronE / electronMinEnergy;
  electronPhi = -gammaPhi;

  fParticleGun->SetParticleEnergy(electronE);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(electronDirection);
  fParticleGun->SetParticleDefinition(fElectronDef); // e-?
  /*
  if (fVerbose > 0)
    G4cout << "Direction: (" << electronDirection.getX()/mm << ","
    << electronDirection.getY()/mm << "," << electronDirection.getZ()/mm << ")\n";
  */
  fAnalysis->SetAsym(GetComptonAsym(rho)); // Asym photon = asym e-
  fAnalysis->SetRho(rho);
  fAnalysis->SetGammaE(electronE/CLHEP::MeV); // Later add SetElectronE
  fAnalysis->SetTheta(57.2957795 * electronTheta / CLHEP::radian);
  fAnalysis->SetPhi(57.2957795 * electronPhi / CLHEP::radian);
  if (fVerbose > 0)
    G4cout << "Shooting electron with energy: " << electronE/CLHEP::MeV << " MeV" << G4endl;

}

void ComptonG4PrimaryGeneratorAction::GeneratePrimaryPolarizedElectronsMode()
{
  fParticleGun->SetParticleEnergy(fElectronEnergy);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
  G4double transAngle = CLHEP::RandFlat::shoot(2*CLHEP::pi)/CLHEP::radian;
  fParticleGun->SetParticlePolarization(G4ThreeVector(
        fTransversePol*std::cos(transAngle),
        fTransversePol*std::sin(transAngle),
        std::sqrt(1.0-(fTransversePol*fTransversePol))));
        //fLongitudinalPol));
  fParticleGun->SetParticleDefinition(fElectronDef);
  fAnalysis->SetAsym(0.0);
  fAnalysis->SetRho(0.0);
  fAnalysis->SetGammaE(0.0);
  fAnalysis->SetTheta(0.0);
  fAnalysis->SetPhi(0.0);
}

/**
 * Returns the theoretical Compton asymmetry for a given normalized photon
 * energy rho.
 *
 * @param rho The normalized photon energy
 */
G4double ComptonG4PrimaryGeneratorAction::GetComptonAsym( G4double rho)
{
  G4double am1 = fAParameter-1.0;
  G4double term1 = rho*rho*am1*am1/(1.+rho*am1);
  G4double term3 = (1.-rho*(1.0+fAParameter))/(1.+rho*am1);
  G4double term4 = 1+rho*am1;
  G4double termdenom = term1+1.0 + term3*term3;
  return (1.0-rho*(1.0+fAParameter))*(1.-1./(term4*term4))/termdenom;
}
