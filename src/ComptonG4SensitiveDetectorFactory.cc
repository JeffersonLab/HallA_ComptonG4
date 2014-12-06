/*
 * ComptonG4SensitiveDetectorFactory.cc
 *
 *  Created on: Dec 04, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4SensitiveDetectorFactory.hh"
#include "ComptonG4Analysis.hh"
#include <G4OpticalPhoton.hh>
#include <G4TrackStatus.hh>
#include <G4VProcess.hh>

ComptonG4SensitiveDetectorFactory::_ComptonG4_SD_Map
  *ComptonG4SensitiveDetectorFactory::fFactoryObjects = 0;
