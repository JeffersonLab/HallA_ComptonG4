// ComptonG4 includes
#include "ComptonG4PrimaryGeneratorAction.hh"
#include "ComptonG4PrimaryGeneratorMessenger.hh"
#include "ComptonG4Analysis.hh"
#include "ComptonConstants.hh"

// GEANT4 Includes
#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <Randomize.hh>

//

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

  fOut.open("out.dat",std::ios::out);
}


void ComptonG4PrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
  G4double laserE = 0.0;      // Laser Photon Energy
  G4double gammaE = 0.0;      // Scattered photon energy
  G4double rho = 0.0;         // Normalized photon energy photonE/CE
  G4double dSig = 0.0;        // The cross section at this energy
  G4double asym = 0.0;        // Theoretical Asymmetry
  G4double gammaTheta = 0.0;  // Photon scattered angle polar angle
  G4double gammaPhi = 0.0;    // Photon scattered azymuthal angle
  G4ThreeVector gammaDirection(0.0,0.0,1.0); // This will hold the direction
              // of the scattered photon. For now, we just point it towards
              // the photon detector

  // We generate the primaries based on the selected mode
  switch(fGeneratorMode) {
  case 1:
    break;
  case 2:
    break;
  case 3: // Compton mode (i.e, real physics mode)
    // Now get a random rho
    rho = GetRandomRho();
    laserE = ComptonConstants::kPlanck *
        ComptonConstants::kC/fLaserWavelength;
    gammaTheta = acos(1-ComptonConstants::kMassElectron*(laserE-rho*fMaxPhotonEnergy)/
        (laserE*rho*fMaxPhotonEnergy));
    gammaPhi=CLHEP::RandFlat::shoot(2.0*pi);
    break;
  default: // Do nothing, use the default GEANT4 gun
    break;
  };
}


void ComptonG4PrimaryGeneratorAction::Initialize()
{
  // Initialize this generator. If we are using compton mode, one of the
  // first thing to initialize is the distribution function (cross section)

  if( fGeneratorMode == 3 ) {
    for(int i = 0; i < 10000; i++ ) {
      // Variables used in the Compton mode
      G4double laserE;  // Laser energy (computed from Wavelength)
      G4double gamma;   //
      G4double rho = G4double(i/10000.0);

      laserE = ComptonConstants::kPlanck *
        ComptonConstants::kC/fLaserWavelength;
      gamma = fElectronEnergy/ComptonConstants::kMassElectron;
      fMaxPhotonEnergy = 4*gamma*gamma*laserE/
        (1.0+(4*gamma*laserE/ComptonConstants::kMassElectron));
      G4double a= 1./
        (1.+4.*fElectronEnergy*laserE/
            (electron_mass_c2*electron_mass_c2));
      G4double am1 = a-1.0;
      G4double term1 = rho*rho*am1*am1/(1.+rho*am1);
      G4double term3 = (1.-rho*(1.0+a))/(1.+rho*am1);
      fCXdSig_dRho[i] = 2*pi*classic_electr_radius*
        classic_electr_radius*a*(term1+1.0+term3*term3);
    }
  }
}

G4double ComptonG4PrimaryGeneratorAction::GetRandomRho()
{
  G4RandGeneral GenDist(fCXdSig_dRho,10000);
  return GenDist.shoot();
}
