/*
 * ComptonG4SensitiveDetectorManager.cc
 *
 *  Created on: Oct 12, 2013
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4SensitiveDetectorManager.hh"
#include "ComptonG4SensitiveDetector.hh"
#include "VComptonG4SensitiveDetector.hh"
#include "ComptonG4SensitiveDetectorFactory.hh"
#include <G4INCLGeant4Compat.hh>
#include <G4SDManager.hh>

ComptonG4SensitiveDetectorManager::ComptonG4SensitiveDetectorManager(ComptonG4Analysis *analysis) :
    fAnalysis(analysis)
{
  ComptonG4SensitiveDetector::SetAnalysis(analysis);
}

ComptonG4SensitiveDetectorManager::~ComptonG4SensitiveDetectorManager()
{
  // Clear detectors
  fDetectors.clear();
}


void ComptonG4SensitiveDetectorManager::RegisterDetector(
    G4LogicalVolume *volume, G4String detType)
{
  G4String name = volume->GetName();
  VComptonG4SensitiveDetector *sd = ComptonG4SensitiveDetectorFactory::
    Create(detType,name);
  if(sd) {
    volume->SetSensitiveDetector(sd);
    G4cout << "Registered SD of type: " << detType
      << " on volume: " << name << G4endl;
    fDetectors.push_back(sd);
    G4SDManager::GetSDMpointer()->AddNewDetector( fDetectors.back() );
  } else {
    G4cerr << "ERROR: SD type " << detType << " not found!" << std::endl;
  }
}

/*
 *
 */
void ComptonG4SensitiveDetectorManager::AddVolume(G4String detName,
    G4VPhysicalVolume *vol)
{
  VComptonG4SensitiveDetector* det = GetDetector(detName);
  if(det)
    det->AddVolume(vol);
}

VComptonG4SensitiveDetector* ComptonG4SensitiveDetectorManager::GetDetector(
    G4String name)
{
  std::vector<VComptonG4SensitiveDetector*>::iterator it;
  for( it = fDetectors.begin(); it != fDetectors.end(); it++ ) {
    if((*it)->GetName() == name)
      return *it;
  }

  // Not found? Boo!
  return 0;
}
