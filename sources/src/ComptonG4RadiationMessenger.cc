//
//  ComptonG4RadiationMessenger.cc
//  
//
//  Created by Tim Hucko on 2017-01-14.
//
//

#include "ComptonG4RadiationMessenger.hh"

#include "ComptonG4Radiation.hh"
#include <G4UIdirectory.hh>
#include <G4UIcmdWithABool.hh>

ComptonG4RadiationMessenger::ComptonG4RadiationMessenger(ComptonG4Radiation *action)
:G4UImessenger(),fAction(action)
{G4cerr << " IN MAIN OF ComptonG4RadiaitonMessenger!!!!!!!!!!" << G4endl;
    
    fSetRadiation = new G4UIdirectory("/Compton/radiation/");
    fSetRadiation->SetGuidance("Set EM radiation");
    
    fSRTypeCmd = new G4UIcmdWithABool("/Compton/radiation/Synchrotron",this);
    fSRTypeCmd->SetGuidance("choose synchtrotron radiation");
    fSRTypeCmd->SetParameterName("Synchrotron", false);
    fSRTypeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fBremsstrahlungCmd = new G4UIcmdWithABool("/Compton/radiation/Bremsstrahlung",this);
    fBremsstrahlungCmd->SetGuidance("choose bremsstrahlung radiation");
    fBremsstrahlungCmd->SetParameterName("Bremsstrahlung", false);
    fBremsstrahlungCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

ComptonG4RadiationMessenger::~ComptonG4RadiationMessenger()
{
    delete fSRTypeCmd;
    delete fBremsstrahlungCmd;
    delete fSetRadiation;
}

void ComptonG4RadiationMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    
    if (command == fSRTypeCmd) {
        fAction->SetSynchrotronRadiation(fSRTypeCmd->GetNewBoolValue(newValue));
    }
    if (command == fBremsstrahlungCmd) {
        fAction->SetBremsstrahlungRadiation(fBremsstrahlungCmd->GetNewBoolValue(newValue));
    }

}
