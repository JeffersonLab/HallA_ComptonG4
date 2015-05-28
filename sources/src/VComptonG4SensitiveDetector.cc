/*
 * VComptonG4SensitiveDetector.cc
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "VComptonG4SensitiveDetector.hh"
#include "ComptonG4Analysis.hh"
#include "ComptonG4OpticalTracker.hh"
#include "ComptonG4Utils.hh"
#include <G4OpticalPhoton.hh>
#include <G4TrackStatus.hh>
#include <G4VProcess.hh>

VComptonG4SensitiveDetector::VComptonG4SensitiveDetector(
    G4String name) :
  G4VSensitiveDetector(name), fAnalysis(0), fOpticalTracker(0)
{
}


VComptonG4SensitiveDetector::~VComptonG4SensitiveDetector()
{
  // TODO Auto-generated destructor stub
}

int VComptonG4SensitiveDetector::GetIndex(G4VPhysicalVolume *vol)
{
  for(unsigned int i = 0; i < fVolumes.size(); i++ ) {
    if(vol==fVolumes[i])
      return i;
  }

  return -1;
}


G4bool VComptonG4SensitiveDetector::ProcessOptionBool(G4String param,
    G4String value, G4String option_name, G4bool &result)
{
  G4bool found = false;
  if( ComptonG4Utils::SameIgnore(param,option_name) ) {
    found = true;
    G4bool boolean;
    if( ComptonG4Utils::GetBool(value,boolean) == 0 ) {
      result = boolean;
      G4cout << "Detector " << GetName() << " option '" <<
        option_name << "' set to '" << (result? "true" : "false") << "'."
        << G4endl;
    } else {
      G4cerr << "Error in Detector " <<  GetName() <<
        ": 'option' " << option_name <<  " should be boolean, but got '"
         << value << "' instead." << G4endl;
    }
  }

  return found;
}

void VComptonG4SensitiveDetector::UnknownOption(G4String param, G4String value)
{
  G4cerr << "Error in Detector " << GetName() << ": Unknown option '" <<
    param << "' ";
  if(!value.empty())
    G4cerr << "with value '" << value << "' ";
  G4cerr << "detected." << G4endl;
}

void VComptonG4SensitiveDetector::ToOpticalTracker(G4Track *track,
    G4bool detected)
{
  if(fOpticalTracker)
    fOpticalTracker->ProcessTrack(track,detected);
}
