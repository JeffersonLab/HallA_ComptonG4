/*
 * ComptonG4OpticalTrackerData.hh
 *
 *  Created on: May 24, 2015
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#ifndef COMPTONG4OPTICALTRACKERDATA_HH
#define COMPTONG4OPTICALTRACKERDATA_HH

#include "VComptonG4Data.hh"
#include <vector>
#include <string>


enum ComptonG4OpticalProcess {
  ComptonG4NoOpticalCreationProcess,
  ComptonG4GenericOptical,
  ComptonG4Cerenkov,
  ComptonG4Scintillation
};

/*
 * The data that can be stored in a rootfile
 */
class ComptonG4OpticalTrackerData : public VComptonG4Data {
public:
  // Starting Information
  Double_t  start_position[3];      // [mm] End position in mm (x,y,z)
  Double_t  start_direction[3];     // [unitless] Momentum direction
  Double_t  start_momentum[3];      // [MeV/c] Momentum

  // Ending Information
  Double_t  end_position[3];      // [mm] End position in mm (x,y,z)
  Double_t  end_direction[3];     // [unitless] Momentum direction
  Double_t  end_momentum[3];      // [MeV/c] Momentum

  Bool_t    detected;         // [unitless] Was this particle detected?

  Double_t  kinetic_energy;   // [MeV] Kinetic Energy
  Double_t  total_length;     // [mm] Total length traveled up to this point
  Int_t     track_id;         // [unitless] ID of this track
  Int_t     parent_id;        // [unitelss] ID of parent track
  Int_t     step_number;      // [unitless] Step number
  ComptonG4OpticalProcess  creation_process;// [enum] Process that created track

  // ROOT stuff
  ComptonG4OpticalTrackerData(){}
  virtual ~ComptonG4OpticalTrackerData(){}
  ClassDef(ComptonG4OpticalTrackerData,1);
};

#endif /* COMPTONG4OPTICALTRACKERDATA_HH */
