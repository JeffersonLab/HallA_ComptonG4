//
//  ComptonG4RadiationMessenger.hh
//  
//
//  Created by Tim Hucko on 2017-01-14.
//
//

#ifndef ____ComptonG4RadiationMessenger__
#define ____ComptonG4RadiationMessenger__

#include <stdio.h>

#include <globals.hh>
#include <G4UImessenger.hh>

class ComptonG4Radiation;
class G4UIdirectory;
class G4UIcmdWithABool;

class ComptonG4RadiationMessenger: public G4UImessenger
{
public:
    
    ComptonG4RadiationMessenger(ComptonG4Radiation*);
   ~ComptonG4RadiationMessenger();
    
   virtual void SetNewValue(G4UIcommand*, G4String);
    
private:
    
    ComptonG4Radiation *fAction;
    
    G4UIdirectory *fSetRadiation;
    G4UIcmdWithABool *fSRTypeCmd;
    G4UIcmdWithABool *fBremsstrahlungCmd;
    
    
};






#endif /* defined(____ComptonG4RadiationMessenger__) */
