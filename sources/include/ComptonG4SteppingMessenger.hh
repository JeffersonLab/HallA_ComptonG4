#ifndef COMPTONG4STEPPINGMESSENGER_HH
#define COMPTONG4STEPPINGMESSENGER_HH

#include <G4UImessenger.hh>

// Pre-defined classes
class ComptonG4SteppingAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

/*!
 * @class ComptonG4SteppingMessenger
 *
 * @brief Primary Generator Messenger
 *
 * Handles user input to configure the primary generator/gun
 *
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4SteppingMessenger : public G4UImessenger {
public:
  ComptonG4SteppingMessenger(ComptonG4SteppingAction *action);
  ~ComptonG4SteppingMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:
  ComptonG4SteppingAction    *fAction;
  G4UIdirectory              *fStepDir;
  G4UIcmdWithADoubleAndUnit  *fSetOpticalMaxStepTimeCmd;
  G4UIcmdWithAnInteger       *fSetVerboseCmd;
  G4UIcmdWithAnInteger       *fSetHitProcessingCmd;
};

#endif
