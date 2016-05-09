
#include "ComptonG4PrimaryGeneratorMessenger.hh"
#include "ComptonG4PrimaryGeneratorAction.hh"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithADouble.hh>

ComptonG4PrimaryGeneratorMessenger::ComptonG4PrimaryGeneratorMessenger(
    ComptonG4PrimaryGeneratorAction *action) :
  fAction(action)
{
  fGunDir = new G4UIdirectory("/Compton/gun/");
  fGunDir->SetGuidance("Controls Primary Particle Generator");

  // Set Mode for primary particle generation
  fGenModeCmd = new G4UIcmdWithAnInteger("/Compton/gun/mode",this);
  fGenModeCmd->SetGuidance("Controls gamma generation mode.");
  fGenModeCmd->SetGuidance("  Choice 1) Mono-Energetic, 2) Flat Distribution, 3) Compton Shape, 4) Bremsstrauhlung");
  fGenModeCmd->SetParameterName("Mode",false);
  fGenModeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Set the Initial Energy (which depends on the mode selected)
  fSetElectronEnergyCmd = new G4UIcmdWithADoubleAndUnit(
      "/Compton/gun/ElectronEnergy",this);
  fSetElectronEnergyCmd->SetGuidance("Initial Elecron Energy (mode dependent).");
  fSetElectronEnergyCmd->SetParameterName("ElectronEnergy",false);
  fSetElectronEnergyCmd->SetUnitCategory("Energy");
  fSetElectronEnergyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Set Laser Wavelenght ( which depends on the mode selected)
  fSetLaserWavelengthCmd = new G4UIcmdWithADoubleAndUnit(
      "/Compton/gun/LaserWavelenght",this);
  fSetLaserWavelengthCmd->SetGuidance("Laser photon wavelenght (mode dependent).");
  fSetLaserWavelengthCmd->SetParameterName("LaserWavelenght",false);
  fSetLaserWavelengthCmd->SetUnitCategory("Length");
  fSetLaserWavelengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Set Initial Photon Vertex
  fSetPhotonVertexXCmd = new G4UIcmdWithADoubleAndUnit(
      "/Compton/gun/PhotonVertexX",this);
  fSetPhotonVertexXCmd->SetGuidance("Initial photon vertex");
  fSetPhotonVertexXCmd->SetParameterName("PhotonVertexX",false);
  fSetPhotonVertexXCmd->SetUnitCategory("Length");
  fSetPhotonVertexXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSetPhotonVertexYCmd = new G4UIcmdWithADoubleAndUnit(
      "/Compton/gun/PhotonVertexY",this);
  fSetPhotonVertexYCmd->SetGuidance("Initial photon vertex");
  fSetPhotonVertexYCmd->SetParameterName("PhotonVertexY",false);
  fSetPhotonVertexYCmd->SetUnitCategory("Length");
  fSetPhotonVertexYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSetPhotonVertexZCmd = new G4UIcmdWithADoubleAndUnit(
      "/Compton/gun/PhotonVertexZ",this);
  fSetPhotonVertexZCmd->SetGuidance("Initial photon vertex");
  fSetPhotonVertexZCmd->SetParameterName("PhotonVertexZ",false);
  fSetPhotonVertexZCmd->SetUnitCategory("Length");
  fSetPhotonVertexZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Set Incident Particle Energy (cannot be used in Compton mode)
  fSetIncidentEnergyCmd = new G4UIcmdWithADoubleAndUnit(
      "/Compton/gun/MaxPhotonEnergy",this);
  fSetIncidentEnergyCmd->SetGuidance("Incident or Max Particle Energy (non compatible with Compton Mode");
  fSetIncidentEnergyCmd->SetParameterName("MaxPhotonEnergy",false);
  fSetIncidentEnergyCmd->SetUnitCategory("Energy");
  fSetIncidentEnergyCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Set Verbose Level
  fSetVerboseCmd = new G4UIcmdWithAnInteger("/Compton/gun/verbose",this);
  fSetVerboseCmd->SetGuidance("Controls verbose level");
  fSetVerboseCmd->SetParameterName("verbose",0);
  fSetVerboseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Set the maximum transverse polarization in beam mode
  fSetTransversePolCmd = new G4UIcmdWithADouble(
      "/Compton/gun/TransversePol",this);
  fSetTransversePolCmd->SetGuidance("Maximum Transverse Polarization in Beam Mode");
  fSetTransversePolCmd->SetParameterName("MaxTransversePol",false);
  fSetTransversePolCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fInitializeCmd = new G4UIcmdWithoutParameter(
      "/Compton/gun/Initialize",this);
}

ComptonG4PrimaryGeneratorMessenger::~ComptonG4PrimaryGeneratorMessenger()
{ // Time for a cleanup!!
  delete fGenModeCmd;
  delete fSetElectronEnergyCmd;
  delete fSetLaserWavelengthCmd;
  delete fSetPhotonVertexZCmd;
  delete fSetIncidentEnergyCmd;
  delete fSetVerboseCmd;
  delete fGunDir;
  delete fInitializeCmd;
}

void ComptonG4PrimaryGeneratorMessenger::SetNewValue(
    G4UIcommand *command, G4String newValue)
{ // Process new value from G4 Kernel
  if( command == fGenModeCmd ) {
    fAction->SetGeneratorMode(fGenModeCmd->GetNewIntValue(newValue));
  } else if ( command == fSetElectronEnergyCmd ) {
    fAction->SetElectronEnergy(fSetElectronEnergyCmd->GetNewDoubleValue(newValue));
  } else if ( command == fSetIncidentEnergyCmd ) {
    fAction->SetMaxPhotonEnergy(fSetIncidentEnergyCmd->GetNewDoubleValue(newValue));
  } else if ( command == fSetLaserWavelengthCmd ) {
    fAction->SetLaserWavelength(fSetLaserWavelengthCmd->GetNewDoubleValue(newValue));
  } else if ( command == fSetPhotonVertexZCmd ) {
    fAction->SetPhotonZ(fSetPhotonVertexZCmd->GetNewDoubleValue(newValue));
  } else if ( command == fSetVerboseCmd ) {
    fAction->SetVerbose(fSetVerboseCmd->GetNewIntValue(newValue));
  } else if ( command == fInitializeCmd ) {
    fAction->Initialize();
  } else if ( command == fSetPhotonVertexXCmd ) {
    fAction->SetPhotonX(fSetPhotonVertexXCmd->GetNewDoubleValue(newValue));
  } else if ( command == fSetPhotonVertexYCmd ) {
    fAction->SetPhotonY(fSetPhotonVertexXCmd->GetNewDoubleValue(newValue));
  } else if ( command == fSetTransversePolCmd ) {
    fAction->SetTransversePol(
        fSetTransversePolCmd->GetNewDoubleValue(newValue));
  }
}
