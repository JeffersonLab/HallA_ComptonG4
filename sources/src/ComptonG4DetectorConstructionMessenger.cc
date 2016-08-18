/*
 * ComptonG4DetectorConstructionMessenger.cc
 *
 *  Created on: November 26, 2012
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4DetectorConstructionMessenger.hh"
#include "ComptonG4DetectorConstruction.hh"
#include "ComptonG4UIcmdWithStringOptions.hh"
#include "ComptonG4GDMLVolumes.hh"

#include <G4UIcommand.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4UIdirectory.hh>
#include <G4UnitsTable.hh>

ComptonG4DetectorConstructionMessenger::ComptonG4DetectorConstructionMessenger(
    ComptonG4DetectorConstruction *det, ComptonG4GDMLVolumes *volMan)
  : fDetector(det), fVolumeManager(volMan)
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

  // Specify whether the world should be placed in a vacuum instead of air
  fWorldInVacuumCmd = new G4UIcmdWithABool("/Compton/geometry/worldInVacuum"
      ,this);
  fWorldInVacuumCmd->SetGuidance(
      "Place the World in a Vacuum as opposed to air");
  fWorldInVacuumCmd->AvailableForStates(G4State_PreInit);

  ////////////////////////////////////////////////////////////////////////////
  // Commands to read from GDML files and add volumes
  // Set curent GDML file to read from
  fReadGDMLFileCmd = new G4UIcmdWithAString("/Compton/geometry/readGDMLFile",
      this);
  fReadGDMLFileCmd->SetGuidance("Read volumes from GDML file");
  fReadGDMLFileCmd->AvailableForStates(G4State_PreInit);
  // Add volume of given name from GDML file
  fAddVolumeCmd = new G4UIcmdWithAString("/Compton/geometry/addVolume" ,this);
  fAddVolumeCmd->SetGuidance("Add a Volume to the World");
  fAddVolumeCmd->AvailableForStates(G4State_PreInit);
  // Specify volume position
  fSetVolumePositionCmd = new G4UIcmdWith3VectorAndUnit(
      "/Compton/geometry/setVolumePosition",this);
  fSetVolumePositionCmd->SetGuidance(
      "Set the position of the last added volume.");
  fSetVolumePositionCmd->SetParameterName("X","Y","Z",false,true);
  fSetVolumePositionCmd->SetUnitCategory("Length");
  fSetVolumePositionCmd->SetDefaultUnit("mm");
  fSetVolumePositionCmd->AvailableForStates(G4State_PreInit);
  // Specify volume theta rotation angle
  fSetVolumeRotationCmd = new G4UIcmdWith3VectorAndUnit(
      "/Compton/geometry/setVolumeRotation",this);
  fSetVolumeRotationCmd->SetGuidance(
      "Set rotation angles (phi, theta, psi)");
  fSetVolumeRotationCmd->SetParameterName("phi","theta","psi",false,true);
  fSetVolumeRotationCmd->SetUnitCategory("Angle");
  fSetVolumeRotationCmd->SetDefaultUnit("rad");
  fSetVolumeRotationCmd->AvailableForStates(G4State_PreInit);
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

  } else if ( command == fWorldInVacuumCmd ) {
    fDetector->SetWorldInVacuum(fWorldInVacuumCmd->GetNewBoolValue(newValue));

  } else if ( command == fReadGDMLFileCmd ) {
    fVolumeManager->SetReadGDMLFile(newValue);

  } else if ( command == fAddVolumeCmd ) {
    fVolumeManager->AddVolume(newValue);

  } else if ( command == fSetVolumePositionCmd ) {
    fVolumeManager->SetVolumePosition(
        fSetVolumePositionCmd->GetNew3VectorValue(newValue));

  } else if ( command == fSetVolumeRotationCmd ) {
    // Get the values as a 3vector from the UI command
    G4ThreeVector rotV =
        fSetVolumeRotationCmd->GetNew3VectorValue(newValue);
    // Now create a new Rotation Matrix from those values
    fVolumeManager->SetVolumeRotation(new G4RotationMatrix(
          rotV[0],rotV[1],rotV[2]));

  }
}

