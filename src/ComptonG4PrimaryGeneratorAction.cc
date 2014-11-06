// ComptonG4 includes
#include "ComptonG4PrimaryGeneratorAction.hh"
#include "ComptonG4PrimaryGeneratorMessenger.hh"
#include "ComptonG4Analysis.hh"

// GEANT4 Includes
#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <Randomize.hh>
#include <G4OpticalPhoton.hh>

// Standard Includes
#include <cmath>

ComptonG4PrimaryGeneratorAction::ComptonG4PrimaryGeneratorAction(ComptonG4Analysis *analysis) :

  fAnalysis(analysis)
{
  G4int n_particle = 1; // Gun shoots photons ( and may shoot electrons too)
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
  fPrimaryVertexLocation = G4ThreeVector(0*mm,0*mm,0*mm);
}

ComptonG4PrimaryGeneratorAction::~ComptonG4PrimaryGeneratorAction()
{
  delete fParticleGun;
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
    GeneratePrimaryComptonMode();
    break;
  case 4: // Optical photon mode
    GeneratePrimaryOpticalMode();
    break;
  default: // Do nothing, use the default GEANT4 gun
    break;
  };
  fParticleGun->GeneratePrimaryVertex(event);
}


void ComptonG4PrimaryGeneratorAction::Initialize()
{
  // Initialize this generator. If we are using compton mode, one of the
  // first thing to initialize is the distribution function (cross section)

  if( fGeneratorMode == 3 ) {
    // Variables used in the Compton mode

    fLaserEnergy = h_Planck *c_light/fLaserWavelength;
    fAParameter = 1/(1+(4*fLaserEnergy*fElectronEnergy)/
        (electron_mass_c2*electron_mass_c2));
    fMaxPhotonEnergy = fElectronEnergy*(1-fAParameter);
    G4double am1 = fAParameter-1.0;
    for(int i = 0; i < 10000; i++ ) {
      G4double rho = G4double(i/10000.0);
      G4double term1 = rho*rho*am1*am1/(1.+rho*am1);
      G4double term3 = (1.-rho*(1.0+fAParameter))/(1.+rho*am1);
      fCXdSig_dRho[i] = 2*pi*classic_electr_radius*
        classic_electr_radius*fAParameter*(term1+1.0+term3*term3);
    }
  }

  G4cout << "Max Scattered Photon Energy: " << fMaxPhotonEnergy/MeV << " MeV" << G4endl;
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
	  fAnalysis->SetGammaE(fMaxPhotonEnergy/MeV);
	  fAnalysis->SetTheta(0.0);
	  fAnalysis->SetPhi(0.0);
}

void ComptonG4PrimaryGeneratorAction::GeneratePrimaryOpticalMode()
{
  G4ThreeVector opticalDirection = G4ThreeVector(0.0,0.0,1.0);
  opticalDirection.setRThetaPhi(1.0,
      CLHEP::RandFlat::shoot(pi/4.0)/radian,
      CLHEP::RandFlat::shoot(2.0*pi)/radian);
  fParticleGun->SetParticleEnergy(
      CLHEP::RandFlat::shoot(1.0)*
      fMaxPhotonEnergy);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(opticalDirection);
  fParticleGun->SetParticlePolarization(opticalDirection);
  fParticleGun->SetParticleDefinition(G4OpticalPhoton::Definition());
}

void ComptonG4PrimaryGeneratorAction::GeneratePrimaryComptonMode()
{
  G4double gammaE = 0.0;      // Scattered photon energy
  G4double rho = 0.0;         // Normalized photon energy photonE/CE
  G4double gammaTheta = 0.0;  // Photon scattered angle polar angle
  G4double gammaPhi = 0.0;    // Photon scattered azymuthal angle
  G4ThreeVector gammaDirection(0.0,0.0,1.0); // This will hold the direction
              // of the scattered photon. For now, we just point it towards
              // the photon detector

  // Now get a random rho
  rho = GetRandomRho();
  gammaE = rho*fMaxPhotonEnergy;
  // I took the following for theta from Megan Friend's CompCal code
  G4double tmp = electron_mass_c2/fElectronEnergy;
  tmp = tmp*tmp +4*gammaE/fElectronEnergy;
  // TODO: Fix this! Why are these values so large?!?!
  gammaTheta = std::acos( 1.0- 0.5*tmp*((1./rho)-1.0));
  // Since the above did not work, I reworked the equation myself
  tmp = electron_mass_c2/fElectronEnergy;
  gammaTheta = std::sqrt(4*fLaserEnergy*fAParameter/gammaE-(tmp*tmp));

  gammaPhi=CLHEP::RandFlat::shoot(2.0*pi);
  gammaDirection.setRThetaPhi(1.0,gammaTheta/radian,gammaPhi/radian);
  fParticleGun->SetParticleEnergy(gammaE);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(gammaDirection);
  fParticleGun->SetParticleDefinition(fGammaDef);
  //G4cout << "Direction: (" << gammaDirection.getX()/mm << ","
  //    << gammaDirection.getY()/mm << "," << gammaDirection.getZ()/mm << ")\n";
  fAnalysis->SetAsym(GetComptonAsym(rho));
  fAnalysis->SetRho(rho);
  fAnalysis->SetGammaE(gammaE/MeV);
  fAnalysis->SetTheta(57.2957795*gammaTheta/radian);
  fAnalysis->SetPhi(57.2957795*gammaPhi/radian);
  if(fVerbose>0)
    G4cout << "Shooting photon with energy: " << gammaE/CLHEP::MeV << " MeV"
      << G4endl;
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
