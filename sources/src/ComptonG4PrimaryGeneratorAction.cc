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

  // Messenger class:
  fGunMessenger = new ComptonG4PrimaryGeneratorMessenger(this);

  // Default mode is a standard GEANT4 gun:
  fGeneratorMode = 0;

  // Initialize all settings to zero (no hard coded values allowed here!!):
  SetPhotonVertex(G4ThreeVector(0.0, 0.0, 0.0));
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

  // Set a default primary Z vertex:
  fPrimaryVertexLocation = G4ThreeVector(0*CLHEP::mm, 0*CLHEP::mm, 0*CLHEP::mm);
}


ComptonG4PrimaryGeneratorAction::~ComptonG4PrimaryGeneratorAction()
{
  if (fParticleGun)
    delete fParticleGun;

  if (fGunMessenger)
    delete fGunMessenger;
}


void ComptonG4PrimaryGeneratorAction::SetGeneratorMode(G4String val)
{
  val.toLower();

  if (val.compareTo("mono") == 0){
    fGeneratorMode = kGenMono;
  }
  else if (val.compareTo("nogen") == 0){
    fGeneratorMode = kGenNoGen;
  }
  else if (val.compareTo("comptonall") == 0){
    fGeneratorMode = kGenComptonAll;
  }
  else if (val.compareTo("optical") == 0){
    fGeneratorMode = kGenOptical;
  }
  else if (val.compareTo("polelectron") == 0){
    fGeneratorMode = kGenPolElectron;
  }
  else if (val.compareTo("comptonelectron") == 0){
    fGeneratorMode = kGenComptonElectron;
  }
  else if (val.compareTo("comptonphoton") == 0){
    fGeneratorMode = kGenComptonPhoton;
  }
  // Else let fGeneratorMode stay 0.

}


void ComptonG4PrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
  // Generate primaries based on selected mode:

  switch(fGeneratorMode) {
  case kGenMono: // Previously case #1
    GeneratePrimaryMonoEnergeticMode();
    break;
  case kGenNoGen:  // Previously case #2
    break;
  case kGenComptonAll: // Compton photon + e- mode (= real physics mode) 
                       // Previously case #3
    GeneratePrimaryComptonMode(event); 
    break;
  case kGenOptical: // Previously case #4
    GeneratePrimaryOpticalMode();
    break;
  case kGenPolElectron: // Previously case #5
    GeneratePrimaryPolarizedElectronsMode();
    break;
  case kGenComptonElectron: // Previously case #6
    GenerateComptonElectronMode();
    break;  
  case kGenComptonPhoton: // Previously case #7
    GenerateComptonPhotonMode();
    break;  
  default: // Do nothing, use the default GEANT4 gun.
    break;
  };
  
  
  
  fParticleGun->GeneratePrimaryVertex(event);
}


void ComptonG4PrimaryGeneratorAction::Initialize()
{
  // Initialize this generator. If we are using Compton mode, one of the
  // first things to initialize is the distribution function (cross section)

  if ( fGeneratorMode == kGenComptonAll 
         || fGeneratorMode == kGenComptonElectron
         || fGeneratorMode == kGenComptonPhoton ) {
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

// ++++++++++++++++++

void ComptonG4PrimaryGeneratorAction::GeneratePrimaryMonoEnergeticMode()
{
  fParticleGun->SetParticleEnergy(fMaxPhotonEnergy);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
  fParticleGun->SetParticleDefinition(fGammaDef);
  fAnalysis->SetAsym(0.0);
  fAnalysis->SetRho(0.0);
  fAnalysis->SetGammaE(fMaxPhotonEnergy / CLHEP::MeV);
  fAnalysis->SetTheta(0.0);
  fAnalysis->SetPhi(0.0);
}

// ++++++++++++++++++

void ComptonG4PrimaryGeneratorAction::GeneratePrimaryComptonMode(G4Event *event)
{

  ComptonG4ComptonGenerator gen = GeneratePrimaryComptonValues();

  fParticleGun->SetParticleEnergy(gen.gammaE);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(gen.gammaDirection);
  fParticleGun->SetParticleDefinition(fGammaDef);
  /*
  if (fVerbose > 0)
    G4cout << "Direction: (" << gammaDirection.getX()/mm << ","
      << gammaDirection.getY()/mm << "," << gammaDirection.getZ()/mm << ")\n";
  */
  fAnalysis->SetAsym(GetComptonAsym(gen.rho));
  fAnalysis->SetRho(gen.rho);
  fAnalysis->SetGammaE(gen.gammaE/CLHEP::MeV);
  fAnalysis->SetTheta(57.2957795 * gen.gammaTheta / CLHEP::radian);
  fAnalysis->SetPhi(57.2957795 * gen.gammaPhi / CLHEP::radian);
  if (fVerbose > 0)
    G4cout << "Shooting photon with energy: " << gen.gammaE/CLHEP::MeV << " MeV"
      << G4endl;

  fParticleGun->GeneratePrimaryVertex(event); // For 2nd particle

  fParticleGun->SetParticleEnergy(gen.electronE);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(gen.electronDirection);
  fParticleGun->SetParticleDefinition(fElectronDef);
}

// ++++++++++++++++++

void ComptonG4PrimaryGeneratorAction::GeneratePrimaryOpticalMode()
{
  G4ThreeVector opticalDirection = G4ThreeVector(0.0,0.0,1.0);
  opticalDirection.setRThetaPhi(1.0,
      CLHEP::RandFlat::shoot(CLHEP::pi / 4.0) / CLHEP::radian,
      CLHEP::RandFlat::shoot(2.0 * CLHEP::pi) / CLHEP::radian);
  fParticleGun->SetParticleEnergy(
      CLHEP::RandFlat::shoot(1.0) * fMaxPhotonEnergy);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(opticalDirection);
  fParticleGun->SetParticlePolarization(opticalDirection);
  fParticleGun->SetParticleDefinition(G4OpticalPhoton::Definition());
}

// ++++++++++++++++++

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

// ++++++++++++++++++

void ComptonG4PrimaryGeneratorAction::GenerateComptonElectronMode(){

  ComptonG4ComptonGenerator gen = GeneratePrimaryComptonValues();

  fParticleGun->SetParticleEnergy(gen.electronE);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(gen.electronDirection);
  fParticleGun->SetParticleDefinition(fElectronDef); 
  /*
  if (fVerbose > 0)
    G4cout << "Direction: (" << electronDirection.getX()/mm << ","
    << electronDirection.getY()/mm << "," << electronDirection.getZ()/mm << ")\n";
  */
  fAnalysis->SetAsym(GetComptonAsym(gen.rho)); // Asym photon = asym e-
  fAnalysis->SetRho(gen.rho);
  fAnalysis->SetGammaE(gen.electronE/CLHEP::MeV); // Later add SetElectronE
  fAnalysis->SetTheta(57.2957795 * gen.electronTheta / CLHEP::radian);
  fAnalysis->SetPhi(57.2957795 * gen.electronPhi / CLHEP::radian);
  if (fVerbose > 0)
    G4cout << "Shooting electron with energy: " << gen.electronE/CLHEP::MeV << " MeV" << G4endl;

}

// ++++++++++++++++++

void ComptonG4PrimaryGeneratorAction::GenerateComptonPhotonMode()
{
  ComptonG4ComptonGenerator gen = GeneratePrimaryComptonValues();

  fParticleGun->SetParticleEnergy(gen.gammaE);
  fParticleGun->SetParticlePosition(fPrimaryVertexLocation);
  fParticleGun->SetParticleMomentumDirection(gen.gammaDirection);
  fParticleGun->SetParticleDefinition(fGammaDef);
  /*
  if (fVerbose > 0)
    G4cout << "Direction: (" << gammaDirection.getX()/mm << ","
      << gammaDirection.getY()/mm << "," << gammaDirection.getZ()/mm << ")\n";
  */
  fAnalysis->SetAsym(GetComptonAsym(gen.rho));
  fAnalysis->SetRho(gen.rho);
  fAnalysis->SetGammaE(gen.gammaE/CLHEP::MeV);
  fAnalysis->SetTheta(57.2957795 * gen.gammaTheta / CLHEP::radian);
  fAnalysis->SetPhi(57.2957795 * gen.gammaPhi / CLHEP::radian);
  if (fVerbose > 0)
    G4cout << "Shooting photon with energy: " << gen.gammaE/CLHEP::MeV << " MeV"
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


ComptonG4ComptonGenerator ComptonG4PrimaryGeneratorAction::GeneratePrimaryComptonValues()
{
  ComptonG4ComptonGenerator result; // Refers to struct in .hh

  result.gammaE = 0.0;      // Scattered photon energy
  result.rho = 0.0;         // Normalized photon energy gammaE/ComptonEdge
  result.gammaTheta = 0.0;  // Photon scattered polar angle
  result.gammaPhi = 0.0;    // Photon scattered azimuthal angle
  result.gammaDirection = G4ThreeVector(0.0, 0.0, 1.0); // This will hold the direction
              // of the scattered photon. 
              // For now, we just point it towards the photon detector.
  result.rho = GetRandomRho();
  result.gammaE = result.rho * fMaxPhotonEnergy;
  // Theta from Megan Friend's CompCal code:
  G4double tmp = CLHEP::electron_mass_c2 / fElectronEnergy;
  result.gammaTheta = std::sqrt( 4 * fLaserEnergy * fAParameter / result.gammaE -(tmp*tmp));
  result.gammaPhi = CLHEP::RandFlat::shoot(2.0 * CLHEP::pi); // Arbitrary
  result.gammaDirection.setRThetaPhi(1.0, result.gammaTheta/CLHEP::radian,
      result.gammaPhi / CLHEP::radian);

  result.electronE = 0.0;    // Scattered electron energy
  result.electronTheta = 0.0; // Scattered electron polar angle
  result.electronDirection = G4ThreeVector(0.0, 0.0, 1.0);
  result.electronP = 0.0;    // Initial electron momentum (z dir) 
  result.electronPhi = 0.0; // Azimuthal scattered electron dir

  result.electronE = (fElectronEnergy + fLaserEnergy) - result.gammaE; // E cons
  result.electronP = std::sqrt(std::pow(fElectronEnergy, 2) - std::pow(CLHEP::electron_mass_c2, 2));	
  result.electronTheta = std::asin(result.gammaE * std::sin(result.gammaTheta) / result.electronP); //Px conservation
  result.electronPhi = -result.gammaPhi;
  result.electronDirection.setRThetaPhi(1.0, result.electronTheta/CLHEP::radian,      result.electronPhi / CLHEP::radian);

  return result;

}
