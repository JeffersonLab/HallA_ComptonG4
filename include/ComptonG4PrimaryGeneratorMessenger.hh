#ifndef COMPTONG4PRIMARYGENERATORMESSENGER_HH
#define COMPTONG4PRIMARYGENERATORMESSENGER_HH

#include <G4UImessenger.hh>

// Pre-defined classes
class ComptonG4PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

/*!
 * @class ComptonG4PrimaryGeneratorMessenger
 *
 * @brief Primary Generator Messenger
 *
 * Handles user input to configure the primary generator/gun
 *
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4PrimaryGeneratorMessenger : public G4UImessenger {
public:
  ComptonG4PrimaryGeneratorMessenger(ComptonG4PrimaryGeneratorAction *action);
  ~ComptonG4PrimaryGeneratorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:
  ComptonG4PrimaryGeneratorAction *fAction;
  G4UIdirectory              *fGunDir;
  G4UIcmdWithAnInteger       *fGenModeCmd;
  G4UIcmdWithADoubleAndUnit  *fSetIncidentEnergyCmd;
  G4UIcmdWithADoubleAndUnit  *fSetElectronEnergyCmd;
  G4UIcmdWithADoubleAndUnit  *fSetLaserWavelengthCmd;
  G4UIcmdWithADoubleAndUnit  *fSetPhotonVertexZCmd;
  G4UIcmdWithoutParameter    *fInitializeCmd;
};

#endif
