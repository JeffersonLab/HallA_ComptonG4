/*
 * ComptonG4RunMessenger.hh
 *
 *  Created on: Nov 26, 2012
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4RUNMESSENGER_HH
#define COMPTONG4RUNMESSENGER_HH

#include <G4UImessenger.hh>

// Pre-defined classes
class ComptonG4RunAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
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
class ComptonG4RunMessenger: public G4UImessenger {
public:
  ComptonG4RunMessenger(ComptonG4RunAction *run);
  virtual ~ComptonG4RunMessenger();

  void SetNewValue(G4UIcommand *command, G4String newvalue);
private:
  ComptonG4RunAction    *fRun;

  G4UIdirectory         *fRunDir;
  G4UIcmdWithAnInteger  *fRunNumberCmd;
  G4UIcmdWithAnInteger  *fRunAutoSaveCmd;
};

#endif /* COMPTONG4RUNMESSENGER_HH */
