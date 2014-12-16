/*
 * ComptonG4EventMessenger.cc
 *
 *  Created on: November 02, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4EventMessenger.hh"
#include "ComptonG4EventAction.hh"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

ComptonG4EventMessenger::ComptonG4EventMessenger(
    ComptonG4EventAction *action) :
  fAction(action)
{
  fEventDir = new G4UIdirectory("/Compton/event/");
  fEventDir->SetGuidance("Control Event Actions");

  // Set Mode for primary particle generation
  fEventPrintCmd = new G4UIcmdWithAnInteger("/Compton/event/printevent",this);
  fEventPrintCmd->SetGuidance("Printout event time details every Nth event ");
  fEventPrintCmd->SetParameterName("event",false);
  // Should always be available, so I've commented the following out (jc2)
  //fEventPrintCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

ComptonG4EventMessenger::~ComptonG4EventMessenger()
{
  if(fEventPrintCmd)
    delete fEventPrintCmd;

  if(fEventDir)
    delete fEventDir;
}

void ComptonG4EventMessenger::SetNewValue(
    G4UIcommand *command, G4String newValue)
{ // Process new value from G4 Kernel
  if( command == fEventPrintCmd ) {
    fAction->SetEventPrintNumber(fEventPrintCmd->GetNewIntValue(newValue));
  }}
