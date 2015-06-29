/*
 * ComptonG4DetectorConstructionMessenger.cc
 *
 *  Created on: November 26, 2012
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4DetectorConstructionMessenger.hh"
#include "ComptonG4DetectorConstruction.hh"
#include "ComptonG4UIcmdWithStringOptions.hh"

#include <G4UIcommand.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIdirectory.hh>

ComptonG4DetectorConstructionMessenger::ComptonG4DetectorConstructionMessenger(
    ComptonG4DetectorConstruction *dete)
  : fDetector(dete)
{
  fGeometryDir = new G4UIdirectory("/Compton/geometry/");
  fGeometryDir->SetGuidance("Controls the setup of the geometry");

  // Set the primary detector specified in the GDML geometry file
  fActivateDetectorCmd = new G4UIcmdWithAString("/Compton/geometry/activeDetector"
      ,this);
  fActivateDetectorCmd->SetGuidance(
      "Activates a detector, and records appropriate data in the tree");
  fActivateDetectorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Initialize Magnetic Field in specified logical volume name
  fMagneticCmd = new ComptonG4UIcmdWithStringOptions(
      "/Compton/geometry/initMagneticField",this,true);
  fMagneticCmd->SetReceiverName("magneticVolume");
  fMagneticCmd->SetGuidance(
      "Set field for specified Magnetic Volume in GDML file");
  fMagneticCmd->SetGuidance(
      "Initialize magnetic field with data from file, and rotate by phi and "
      "theta");
  fMagneticCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

ComptonG4DetectorConstructionMessenger::~ComptonG4DetectorConstructionMessenger()
{
  if(fMagneticCmd)
    delete fMagneticCmd;

  if(fActivateDetectorCmd)
    delete fActivateDetectorCmd;

  if(fGeometryDir)
    delete fGeometryDir;
}

void ComptonG4DetectorConstructionMessenger::SetNewValue(
    G4UIcommand *command, G4String newValue )
{
  if( command == fActivateDetectorCmd ) {
    fDetector->ActivateDetector(newValue);
  } else if ( command == fMagneticCmd ) {
    fDetector->ProcessMagneticVolumeOptions(fMagneticCmd->GetReceiver(),
        fMagneticCmd->GetOptions());
  }
}

