/*
 * ComptonG4SensitiveDetectorMessenger.cc
 *
 *  Created on: November 26, 2012
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4SensitiveDetectorMessenger.hh"
#include "ComptonG4SensitiveDetectorManager.hh"
#include "ComptonG4UIcmdWithStringOptions.hh"

#include <G4UIcommand.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIdirectory.hh>

ComptonG4SensitiveDetectorMessenger::ComptonG4SensitiveDetectorMessenger(
    ComptonG4SensitiveDetectorManager *man)
  : fSDManager(man)
{
  fDetectorDir = new G4UIdirectory("/Compton/detector/");
  fDetectorDir->SetGuidance("Control Sensitive Detectors");

  // Set the primary detector specified in the GDML geometry file
  fActivateDetectorCmd = new G4UIcmdWithAString(
      "/Compton/detector/activeDetector"
      ,this);
  fActivateDetectorCmd->SetGuidance(
      "Activates a detector, and records appropriate data in the tree");
  fActivateDetectorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Set option for named detector
  fSetOptionCmd = new ComptonG4UIcmdWithStringOptions(
      "/Compton/detector/setoptions",this,true);
  fSetOptionCmd->SetReceiverName("detectorName");
  fSetOptionCmd->SetGuidance(
      "Set Option for specified named detector (as named on the GDML file)");
  fSetOptionCmd->SetGuidance("     detectorName : name of detector");
  fSetOptionCmd->SetGuidance("     options : options to pass to detector.");
}

ComptonG4SensitiveDetectorMessenger::~ComptonG4SensitiveDetectorMessenger()
{
  // Cleanup detector messenger
  if(fSetOptionCmd)
    delete fSetOptionCmd;

  if(fActivateDetectorCmd)
    delete fActivateDetectorCmd;

  if(fDetectorDir)
    delete fDetectorDir;
}

void ComptonG4SensitiveDetectorMessenger::SetNewValue(
    G4UIcommand *command, G4String )
{
  if( command == fActivateDetectorCmd ) {
    //fDetector->ActivateDetector(newValue);
  } else if ( command == fSetOptionCmd ) {
    fSDManager->SetDetectorOptions(fSetOptionCmd->GetReceiver(),
        fSetOptionCmd->GetOptions());
  }
}
