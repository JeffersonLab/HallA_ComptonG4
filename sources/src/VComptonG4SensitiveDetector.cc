/*
 * VComptonG4SensitiveDetector.cc
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "VComptonG4SensitiveDetector.hh"
#include "ComptonG4Analysis.hh"
#include <G4OpticalPhoton.hh>
#include <G4TrackStatus.hh>
#include <G4VProcess.hh>

VComptonG4SensitiveDetector::VComptonG4SensitiveDetector(
    G4String name) :
  G4VSensitiveDetector(name), fAnalysis(0)
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
