#include "ComptonG4SteppingMessenger.hh"
#include "ComptonG4SteppingAction.hh"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAnInteger.hh>

ComptonG4SteppingMessenger::ComptonG4SteppingMessenger(
    ComptonG4SteppingAction *action) :
  fAction(action)
{
  fStepDir = new G4UIdirectory("/Compton/step/");
  fStepDir->SetGuidance("Controls Stepping Action");

  // Set the Initial Energy (which depends on the mode selected)
  fSetOpticalMaxStepTimeCmd = new G4UIcmdWithADoubleAndUnit(
      "/Compton/step/OpticalMaxStepTime",this);
  fSetOpticalMaxStepTimeCmd->SetGuidance("Set the max step time for optical photons.");
  fSetOpticalMaxStepTimeCmd->SetParameterName("OpticalMaxStepTime",false);
  fSetOpticalMaxStepTimeCmd->SetUnitCategory("Time");
  fSetOpticalMaxStepTimeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Set Verbose Level
  fSetVerboseCmd = new G4UIcmdWithAnInteger("/Compton/step/verbose",this);
  fSetVerboseCmd->SetGuidance("Controls verbose level");
  fSetVerboseCmd->SetParameterName("verbose",0);
  fSetVerboseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Set Hit Processing Level
  fSetHitProcessingCmd = new G4UIcmdWithAnInteger("/Compton/step/hitProcessLevel",this);
  fSetHitProcessingCmd->SetGuidance("Controls hit processing level");
  fSetHitProcessingCmd->SetParameterName("hitProc",0);
  fSetHitProcessingCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

ComptonG4SteppingMessenger::~ComptonG4SteppingMessenger()
{ // Time for a cleanup!!
  if(fSetOpticalMaxStepTimeCmd) delete fSetOpticalMaxStepTimeCmd;
  if(fStepDir)                  delete fStepDir;
  if(fSetOpticalMaxStepTimeCmd) delete fSetOpticalMaxStepTimeCmd;
  if(fSetVerboseCmd)            delete fSetVerboseCmd;
  if(fSetHitProcessingCmd)      delete fSetHitProcessingCmd;
}

void ComptonG4SteppingMessenger::SetNewValue(
    G4UIcommand *command, G4String newValue)
{ // Process new value from G4 Kernel
  if ( command == fSetOpticalMaxStepTimeCmd ) {
    fAction->SetOpticalMaxStepTime(fSetOpticalMaxStepTimeCmd->GetNewDoubleValue(newValue));
  } else if ( command == fSetVerboseCmd ) {
    fAction->SetVerbose(fSetVerboseCmd->GetNewIntValue(newValue));
  } else if ( command == fSetHitProcessingCmd ) {
    fAction->SetHitProcessingLevel(fSetHitProcessingCmd->GetNewIntValue(newValue));
  }
}
