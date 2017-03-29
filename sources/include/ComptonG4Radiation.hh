//
//  ComptonG4Radiation.hh
//  
//
//  Created by Tim Hucko on 2017-01-10.
//
//

#ifndef ____ComptonG4Radiation__
#define ____ComptonG4Radiation__

#include <stdio.h>
#include <G4VUserPhysicsList.hh>
#include <globals.hh>
#include <G4Step.hh>


class ComptonG4RadiationMessenger;
class ComptonG4Analysis;
class G4VPhysicalVolume;
class G4Step;



class ComptonG4Radiation: public G4VUserPhysicsList
{
  public:
    ComptonG4Radiation();
   ~ComptonG4Radiation();
    
    
    virtual void ConstructParticle();
    void ConstructLeptons();
    void ConstructBosons();
    
    
    void SetSynchrotronRadiation(G4bool val) {fSRType = val;};
    void SetBremsstrahlungRadiation(G4bool val) {fBremsstrahlung = val;};
    
    virtual void SetCuts();
        
    virtual void ConstructProcess();
    void ConstructGeneral();
    void ConstructRad();
   
    
    
  
    
private:
    
    ComptonG4RadiationMessenger *fRadiationMessenger;
    G4bool fBremsstrahlung;
    G4bool fSRType;
 
    G4ThreeVector pos1;
    G4ThreeVector pos2;
   
    
   
    
};

#endif /* defined(____ComptonG4Radiation__) */
