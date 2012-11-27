/*
 * ComptonG4DetectorConstructionMessenger.hh
 *
 *  Created on: Nov 26, 2012
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4DETECTORCONSTRUCTIONMESSENGER_HH_
#define COMPTONG4DETECTORCONSTRUCTIONMESSENGER_HH_

#include <G4UImessenger.hh>

/*!
 * @class ComptonG4PDetectorConstructionMessenger
 *
 * @brief Detector Construction Messenger class
 *
 * Defines the Messenger class for the Compton Detector Constructor
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4DetectorConstructionMessenger: public G4UImessenger {
public:
  ComptonG4DetectorConstructionMessenger(ComptonG4DetectorConstruction *dete);
  virtual ~ComptonG4DetectorConstructionMessenger();
};

#endif /* COMPTONG4DETECTORCONSTRUCTIONMESSENGER_HH_ */
