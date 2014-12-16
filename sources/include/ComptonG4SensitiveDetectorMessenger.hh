/*
 * ComptonG4SensitiveDetectorMessenger.hh
 *
 *  Created on: Nov 26, 2012
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4SENSITIVEDETECTORMESSENGER_HH_
#define COMPTONG4SENSITIVEDETECTORMESSENGER_HH_

#include <G4UImessenger.hh>

// Pre-defined classes
class ComptonG4SensitiveDetectorManager;
class G4UIdirectory;
class G4UIcmdWithAString;
class ComptonG4UIcmdWithStringOptions;

/*!
 * @class ComptonG4PSensitiveDetectorMessenger
 *
 * @brief Sensitive Detector Messenger class
 *
 * Defines the Messenger class for the Compton Detector Constructor
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4SensitiveDetectorMessenger: public G4UImessenger {
public:
  ComptonG4SensitiveDetectorMessenger(ComptonG4SensitiveDetectorManager *man);
  virtual ~ComptonG4SensitiveDetectorMessenger();

  void SetNewValue(G4UIcommand *command, G4String newValues);
private:
  ComptonG4SensitiveDetectorManager *fSDManager;

  G4UIdirectory                     *fDetectorDir;
  G4UIcmdWithAString                *fActivateDetectorCmd;
  ComptonG4UIcmdWithStringOptions   *fSetOptionCmd;
};

#endif /* COMPTONG4SENSITIVEDETECTORMESSENGER_HH_ */
