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
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

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
  ComptonG4DetectorConstructionMessenger(ComptonG4DetectorConstruction *dete);
  virtual ~ComptonG4DetectorConstructionMessenger();

  void SetNewValue(G4UIcommand *command, G4String newvalue);
private:
  ComptonG4DetectorConstruction *fDetector;

  G4UIdirectory                 *fGeometryDir;
  G4UIcmdWithAString            *fActivateDetectorCmd;
};

#endif /* COMPTONG4DETECTORCONSTRUCTIONMESSENGER_HH_ */
