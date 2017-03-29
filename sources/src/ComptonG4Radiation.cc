//
//  ComptonG4Radiation.cc
//  
//
//  Created by Tim Hucko on 2017-01-10.
//
//


#include "ComptonG4Radiation.hh"
#include "ComptonG4RadiationMessenger.hh"
#include "ComptonG4Analysis.hh"
#include <G4ProcessManager.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTypes.hh>
#include <G4ParticleTable.hh>
#include <G4VSolid.hh>

#include <G4Decay.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <iostream>
#include <fstream>
#include <G4SynchrotronRadiation.hh>
#include <G4eBremsstrahlung.hh>
#include <G4VPhysicalVolume.hh>
#include <G4Track.hh>

#include <G4StepLimiter.hh>
#include <G4GDMLParser.hh>






ComptonG4Radiation::ComptonG4Radiation()
: G4VUserPhysicsList()

{  G4cerr << " IN MAIN OF ComptonG4Radiation!!!!!!!!!!" << G4endl;
   
   
   fRadiationMessenger = new ComptonG4RadiationMessenger(this);
    defaultCutValue = 1.*CLHEP::m;
    fSRType = false;
    fBremsstrahlung = true;
    
   
    
}

ComptonG4Radiation::~ComptonG4Radiation()
{
    delete fRadiationMessenger;
}


void ComptonG4Radiation::ConstructParticle()
{
    
    ConstructBosons();
    ConstructLeptons();
}

void ComptonG4Radiation::ConstructBosons()
{
    
    G4Gamma::GammaDefinition();
}



void ComptonG4Radiation::ConstructLeptons()
{
    G4Electron::ElectronDefinition();
   
}


void ComptonG4Radiation::ConstructProcess()
{ 
   // G4cerr << " In construct Process" << G4endl;
  
    AddTransportation();
    ConstructRad();
    ConstructGeneral();
    
}

void ComptonG4Radiation::ConstructRad()
{
 
    
    
  
    
    theParticleIterator->reset();
    while( (*theParticleIterator)() ){
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();
        
       
        if(fSRType){
        
    pmanager->AddProcess(new G4SynchrotronRadiation,  -1,-1,4);
            
        }
        
        if(fBremsstrahlung){
        
        pmanager->AddProcess(new G4eBremsstrahlung,  -1, 3, 3);
            
        }
        
    }
}




void ComptonG4Radiation::ConstructGeneral()
{
    
 
    G4Decay* theDecayProcess = new G4Decay();
    theParticleIterator->reset();
    while ((*theParticleIterator)()){
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        if (theDecayProcess->IsApplicable(*particle)) {
            pmanager ->AddProcess(theDecayProcess);
           
            pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
            pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
        }
    }
}

void ComptonG4Radiation::SetCuts()
{
    if (verboseLevel >0){
       // G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
    }
    SetCutValue(defaultCutValue, "gamma");
    SetCutValue(defaultCutValue, "e-");
    SetCutValue(defaultCutValue, "e+");
    
    if (verboseLevel >0) DumpCutValuesTable();
}
