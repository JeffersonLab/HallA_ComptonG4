#ifndef COMPTONG4EVENTMESSENGER_HH
#define COMPTONG4EVENTMESSENGER_HH

#include <G4UImessenger.hh>

// Pre-defined classes
class ComptonG4EventAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

/*!
 * @class ComptonG4EventMessenger
 *
 * @brief Event Action Messenger
 *
 * Handles user input to configure the event actions
 *
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4EventMessenger : public G4UImessenger {
public:
  ComptonG4EventMessenger(ComptonG4EventAction *action);
  ~ComptonG4EventMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:
  ComptonG4EventAction *fAction;
  G4UIdirectory              *fEventDir;
  G4UIcmdWithAnInteger       *fEventPrintCmd;
};

#endif
