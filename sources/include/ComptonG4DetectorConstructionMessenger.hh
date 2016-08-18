/*
 * ComptonG4DetectorConstructionMessenger.hh
 *
 *  Created on: Nov 26, 2012
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4DETECTORCONSTRUCTIONMESSENGER_HH_
#define COMPTONG4DETECTORCONSTRUCTIONMESSENGER_HH_

#include <G4UImessenger.hh>

// Pre-defined classes
class ComptonG4DetectorConstruction;
class ComptonG4GDMLVolumes;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWith3VectorAndUnit;
class ComptonG4UIcmdWithStringOptions;

/*!
 * @class ComptonG4PDetectorConstructionMessenger
 *
 * @brief Detector Construction Messenger class
 *
 * Defines the Messenger class for the Compton Detector Constructor
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4DetectorConstructionMessenger: public G4UImessenger {
public:
  ComptonG4DetectorConstructionMessenger(ComptonG4DetectorConstruction *det,
      ComptonG4GDMLVolumes *volMan);
  virtual ~ComptonG4DetectorConstructionMessenger();

  void SetNewValue(G4UIcommand *command, G4String newvalue);
private:
  ComptonG4DetectorConstruction *fDetector;
  ComptonG4GDMLVolumes          *fVolumeManager;

  G4UIdirectory                 *fGeometryDir;
  G4UIcmdWithAString            *fActivateDetectorCmd;
  ComptonG4UIcmdWithStringOptions *fMagneticCmd;
  G4UIcmdWithABool              *fWorldInVacuumCmd;
  G4UIcmdWithAString            *fReadGDMLFileCmd;
  G4UIcmdWithAString            *fAddVolumeCmd;
  G4UIcmdWith3VectorAndUnit     *fSetVolumePositionCmd;
  G4UIcmdWith3VectorAndUnit     *fSetVolumeRotationCmd;
};

#endif /* COMPTONG4DETECTORCONSTRUCTIONMESSENGER_HH_ */
