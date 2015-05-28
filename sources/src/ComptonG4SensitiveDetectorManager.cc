/*
 * ComptonG4SensitiveDetectorManager.cc
 *
 *  Created on: Oct 12, 2013
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4SensitiveDetectorManager.hh"
#include "VComptonG4SensitiveDetector.hh"
#include "ComptonG4SensitiveDetectorFactory.hh"
#include "ComptonG4SensitiveDetectorMessenger.hh"
#include <G4INCLGeant4Compat.hh>
#include <G4SDManager.hh>
#include <ComptonG4Utils.hh>


// Strings of special option specifiers
const G4String ComptonG4SensitiveDetectorManager::fOptionsAll =
  ":ALL:"; // Apply to all (reject bad options)
const G4String ComptonG4SensitiveDetectorManager::fOptionsAllIgnore =
  ":ALL_IGNORE:"; // Apply to all (ignore uknown)

ComptonG4SensitiveDetectorManager::ComptonG4SensitiveDetectorManager(
    ComptonG4Analysis *analysis, ComptonG4OpticalTracker *optical_tracker) :
  fMessenger(0), fAnalysis(analysis), fOpticalTracker(optical_tracker)
{
  fMessenger = new ComptonG4SensitiveDetectorMessenger(this);
}

ComptonG4SensitiveDetectorManager::~ComptonG4SensitiveDetectorManager()
{
  // Clear detectors
  fDetectors.clear();

  if(fMessenger)
    delete fMessenger;
}


void ComptonG4SensitiveDetectorManager::RegisterDetector(
    G4LogicalVolume *volume, G4String detType)
{
  G4String name = volume->GetName();
  VComptonG4SensitiveDetector *sd = ComptonG4SensitiveDetectorFactory::
    Create(detType,name);
  if(sd) {
    sd->SetAnalysis(fAnalysis);
    sd->SetOpticalTracker(fOpticalTracker);
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

/*
 *
 */

void ComptonG4SensitiveDetectorManager::SetDetectorOptions(
    G4String name, std::map<G4String,G4String> options)
{
  bool apply_to_all = false;
  bool ignore_unknown = false;

  // Special cases
  if( ComptonG4Utils::SameIgnore(name,fOptionsAll) ) {
    apply_to_all = true;
    ignore_unknown = false;
  } else if( ComptonG4Utils::SameIgnore(name,fOptionsAllIgnore) ) {
    apply_to_all = true;
    ignore_unknown = true;
  }
  if(apply_to_all) {
    std::vector<VComptonG4SensitiveDetector*>::iterator it;
    for( it = fDetectors.begin(); it != fDetectors.end(); it++ ) {
      (*it)->SetOptions(options,ignore_unknown);
    }

  } else {
    // Check that the detector is registered already
    VComptonG4SensitiveDetector *det = GetDetector(name);
    if(!det) {
      G4cerr << "Error in ComptonG4SensitiveDetectorManager::SetDetectorOptions()"
        << " detector named " << name << " is not registered. Options ignored!"
        << G4endl;
      std::vector<VComptonG4SensitiveDetector*>::iterator it;
      return;
    } else {
      det->SetOptions(options,false);
    }
  }
}
