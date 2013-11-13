/*
 * ComptonG4SensitiveDetectorManager.cc
 *
 *  Created on: Oct 12, 2013
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4SensitiveDetectorManager.hh"
#include "ComptonG4SensitiveDetector.hh"
#include <G4INCLGeant4Compat.hh>
#include <G4SDManager.hh>

ComptonG4SensitiveDetectorManager::ComptonG4SensitiveDetectorManager(ComptonG4Analysis *analysis) :
    fAnalysis(analysis)
{

}

ComptonG4SensitiveDetectorManager::~ComptonG4SensitiveDetectorManager()
{
  // TODO Auto-generated destructor stub
}


G4VSensitiveDetector* ComptonG4SensitiveDetectorManager::RegisterDetector(G4String name)
{
  G4cout << "Registering new Sensitive Detector:  " << name << G4endl;
  fDetectors.push_back(new ComptonG4SensitiveDetector(name));
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector( fDetectors.back() );
  return fDetectors.back();
}
